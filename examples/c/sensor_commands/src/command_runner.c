#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>

#include <sensor_commands/command.h>
#include <sensor_commands/command_runner.h>
#include <sensor_commands/external/utlist.h>

/** Command queue node structure */
struct CmdQNode {
    struct Command *cmd;
    /** Node pointers to support doubly linked lists */
    struct CmdQNode *next, *prev;
};

/** Command queue structure */
struct CmdQueue {
    struct CmdQNode *head;
    /** Current number of commands in the queue */
    int len;
    /** Maximum number of commands in the queue */
    int max_len;
    /** Semaphore to signal there is empty space in the queue to send new commands */
    sem_t empty;
    /** Semaphore to signal there is some command in the queue */
    sem_t occupied;
    /** Mutex lock to protect access to the queue nodes */
    pthread_mutex_t lock;
};

/** Command runner structure */
struct CommandRunner {
    /** ID for the command consumer thread */
    pthread_t tid;
    /** Queue to store commands to be processed by the consumer thread */
    struct CmdQueue queue;
};

static int cmd_queue_init(struct CmdQueue *queue,
                          int max_len)
{
    int ret;

    if (max_len >= SEM_VALUE_MAX) {
        fprintf(stderr, "Queue max length %d exceeds SEM_VALUE_MAX=%d\n",
                max_len, SEM_VALUE_MAX);
        return ERANGE;
    }

    queue->head = NULL;
    queue->len = 0;
    queue->max_len = max_len;

    ret = sem_init(&queue->empty, 0, max_len);
    if (ret) return ret;

    ret = sem_init(&queue->occupied, 0, 0);
    if (ret) {
        sem_destroy(&queue->empty);
        return ret;
    }

    ret = pthread_mutex_init(&queue->lock, NULL);
    if (ret) {
        sem_destroy(&queue->occupied);
        sem_destroy(&queue->empty);
        return ret;
    }
    return 0;
}

static void cmd_queue_uninit(struct CmdQueue *queue)
{
    sem_destroy(&queue->occupied);
    sem_destroy(&queue->empty);
    pthread_mutex_destroy(&queue->lock);
}

static int cmd_queue_send(struct CmdQueue *queue,
                          struct Command *cmd)
{
    int ret, err_ret;

    struct CmdQNode *qnode = malloc(sizeof(struct CmdQNode));
    if (qnode == NULL) {
        fprintf(stderr, "Failed to allocate command queue node\n");
        return ENOMEM;
    }
    qnode->cmd = cmd;

    // Wait for queue to have some empty space
    sem_wait(&queue->empty);

    // Take lock before accessing queue nodes
    ret = pthread_mutex_lock(&queue->lock);
    if (ret) return ret;

    if (queue->len >= queue->max_len) {
        ret = ERANGE;
        fprintf(stderr, "No more space in queue, length exceeds max of %d\n",
                queue->max_len);
        // Always unlock mutex before returning
        err_ret = pthread_mutex_unlock(&queue->lock);
        if (err_ret) {
            fprintf(stderr, "Failed to unlock queue mutex with %d\n", err_ret);
        }
        return ret;
    }

    // Append qnode to the tail of the queue
    DL_APPEND(queue->head, qnode);
    queue->len++;

    ret = pthread_mutex_unlock(&queue->lock);
    if (ret) return ret;

    // Notify receiver of queue holding some commands
    sem_post(&queue->occupied);

    return 0;
}

static int cmd_queue_recv(struct CmdQueue *queue,
                          struct Command **cmd)
{
    int ret;
    struct CmdQNode *qnode;

    // Wait for queue to store some command
    sem_wait(&queue->occupied);

    // Take lock before accessing queue nodes
    ret = pthread_mutex_lock(&queue->lock);
    if (ret) return ret;

    // The head node should never be NULL if the queue is occupied
    assert(queue->head != NULL);

    // Pop node from the head of the queue
    qnode = queue->head;
    DL_DELETE(queue->head, qnode);
    queue->len--;

    // Get command pointer from node
    *cmd = qnode->cmd;

    // Release queue node memory
    free(qnode);

    ret = pthread_mutex_unlock(&queue->lock);
    if (ret) return ret;

    sem_post(&queue->empty);
    return 0;
}

static void *cmd_runner_consumer(void *data)
{
    int ret;
    struct CommandRunner *cmd_runner = data;
    struct Command *cmd = NULL;
    while (true) {
        ret = cmd_queue_recv(&cmd_runner->queue, &cmd);
        if (ret) {
            fprintf(stderr, "Failed to receive command from queue with ret=%d\n", ret);
            return NULL;
        }
        if (cmd->execute == NULL) {
            // Treat command with NULL execute function as stop signal
            return NULL;
        }
        // For now just execute the command immediately
        // TODO: Implement ratelimiter
        command_execute(cmd);
    }
    return NULL;
}

struct CommandRunner * command_runner_create(struct CommandRunnerConfig *cfg)
{
    int ret;
    struct CommandRunner *cmd_runner;

    if (cfg == NULL) {
        fprintf(stderr, "Command runner config is NULL\n");
        return NULL;
    }
    cmd_runner = (struct CommandRunner *)calloc(1, sizeof(struct CommandRunner));
    if (cmd_runner == NULL) {
        fprintf(stderr, "Failed to allocate command runner\n");
        return NULL;
    }
    ret = cmd_queue_init(&cmd_runner->queue, cfg->q_max_size);
    if (ret) {
        fprintf(stderr, "Failed to init command queue with ret=%d\n", ret);
        free(cmd_runner);
        return NULL;
    }
    return cmd_runner;
}

int command_runner_start(struct CommandRunner *cmd_runner)
{
    int ret;

    ret = pthread_create(&cmd_runner->tid, NULL, cmd_runner_consumer, cmd_runner);
    if (ret) {
        fprintf(stderr, "Failed to create command runner thread\n");
        return ret;
    }
    return 0;
}

int command_runner_stop(struct CommandRunner *cmd_runner)
{
    int ret;
    struct Command cmd = {};

    // Send special command (execute == NULL) as stop signal
    ret = cmd_queue_send(&cmd_runner->queue, &cmd);
    if (ret) {
        fprintf(stderr, "Failed to send stop command\n");
        return ret;
    }

    ret = pthread_join(cmd_runner->tid, NULL);
    if (ret) {
        fprintf(stderr, "Failed to join command runner thread\n");
        return ret;
    }
    return 0;
}

int command_runner_send(struct CommandRunner *cmd_runner,
                        struct Command *cmd)
{
    return cmd_queue_send(&cmd_runner->queue, cmd);
}

void command_runner_destroy(struct CommandRunner *cmd_runner)
{
    cmd_queue_uninit(&cmd_runner->queue);
    free(cmd_runner);
}
