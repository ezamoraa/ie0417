#include "gtest/gtest.h"
#include "MQTTClient.h"

#include <testutil/rand_gen.hpp>

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientPub"
#define QOS         1
#define TIMEOUT     10000L
#define STR_MAX_SIZE  100

void msg_delivered_cb(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
}

int msg_arrived_cb(void *context, char *topicName, int topicLen,
                   MQTTClient_message *message)
{
    char* payload;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payload = (char *)message->payload;
    for(int i=0; i < message->payloadlen; i++) {
        putchar(*payload++);
    }
    putchar('\n');
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void conn_lost_cb(void *context, char *cause)
{
    printf("Connection lost\n");
    printf("     cause: %s\n", cause);
}

/** Example fixture class for demo API tests */
class mqtt_client_fixture : public testing::Test
{
 protected:
    /* Fixture class members, accesible from test functions */
    int value;
    testutil::rand_gen rng;
    MQTTClient client;

    /* Fixture class constructor */
    mqtt_client_fixture()
        : value(2), rng(rand())
    {
        std::cout << "Test fixture constructor! "<< std::endl;
        std::cout << "  RNG seed " << rng.get_seed() << std::endl;
    }

    virtual void SetUp() {
        int ret;
        MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

        MQTTClient_create(&client, ADDRESS, CLIENTID,
                          MQTTCLIENT_PERSISTENCE_NONE, NULL);
        conn_opts.keepAliveInterval = 20;
        conn_opts.cleansession = 1;
        ASSERT_NE(client, nullptr);

        ret = MQTTClient_setCallbacks(client, NULL, conn_lost_cb, msg_arrived_cb, msg_delivered_cb);
        ASSERT_EQ(ret, 0);

        ret = MQTTClient_connect(client, &conn_opts);
        ASSERT_EQ(ret, MQTTCLIENT_SUCCESS) << "Failed to connect, ret: " << ret << std::endl;
    }

    virtual void TearDown() {
        int ret;

        ret = MQTTClient_disconnect(client, 10000);
        ASSERT_EQ(ret, MQTTCLIENT_SUCCESS);

        MQTTClient_destroy(&client);
    }
};

/** Test the synchronous publication of an MQTT message */
TEST_F(mqtt_client_fixture, publish_synchronous)
{
    int ret = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    const char topic[STR_MAX_SIZE] = "example/test";
    char msg[STR_MAX_SIZE];

    int num_a = (int)rng.get_rnd_u64();
    int num_b = (int)rng.get_rnd_u64();

    ret = snprintf(msg, STR_MAX_SIZE, "Hello!: Num A: %d, Num B: %d", num_a, num_b);
    ASSERT_GT(ret, 0);

    pubmsg.payload = msg;
    pubmsg.payloadlen = strlen(msg);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    ret = MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    ASSERT_EQ(ret, MQTTCLIENT_SUCCESS) << "Failed to publish, ret: " << ret << std::endl;

    printf("Waiting for up to %d seconds for publication of message\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), topic, CLIENTID);

    ret = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    ASSERT_EQ(ret, MQTTCLIENT_SUCCESS);

    printf("Message with delivery token %d delivered\n", token);
}

/** Test the asynchronous reception of an MQTT message on a subscribed topic */
TEST_F(mqtt_client_fixture, subscribe_asynchronous)
{
    int ret = 0;
    const char topic[STR_MAX_SIZE] = "example/test";
    char ch;

    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
           "Press Q<Enter> to quit\n\n", topic, CLIENTID, QOS);
    ret = MQTTClient_subscribe(client, topic, QOS);
    ASSERT_EQ(ret, 0);

    do {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');
}
