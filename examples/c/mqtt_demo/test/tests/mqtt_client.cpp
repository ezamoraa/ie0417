#include "gtest/gtest.h"
#include "MQTTClient.h"

#include <testutil/rand_gen.hpp>

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "MQTT Examples"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

/** Example fixture class for demo API tests */
class mqtt_client_fixture : public testing::Test
{
 protected:
    /* Fixture class members, accesible from test functions */
    int value;
    testutil::rand_gen rng;

    /* Fixture class constructor */
    mqtt_client_fixture()
        : value(2), rng(rand())
    {
        std::cout << "Test fixture constructor! "<< std::endl;
        std::cout << "  RNG seed " << rng.get_seed() << std::endl;
    }

    virtual void SetUp() {
        std::cout << "Test fixture SetUp! "<< std::endl;
    }

    virtual void TearDown() {
        std::cout << "Test fixture TearDown! "<< std::endl;
    }
};

/** Test the synchronous publication of an MQTT message */
TEST_F(mqtt_client_fixture, publish_synchronous)
{
    int ret = 0;

    int num_a = (int)rng.get_rnd_u64();
    int num_b = (int)rng.get_rnd_u64();
    std::cout << "  Num A: " << num_a << std::endl;
    std::cout << "  Num B: " << num_b << std::endl;

    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    ret = MQTTClient_connect(client, &conn_opts);
    ASSERT_EQ(ret, MQTTCLIENT_SUCCESS) << "Failed to connect, ret: " << ret << std::endl;

    pubmsg.payload = (void *)PAYLOAD;
    pubmsg.payloadlen = strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    ret = MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    ASSERT_EQ(ret, MQTTCLIENT_SUCCESS) << "Failed to publish, ret: " << ret << std::endl;

    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);

    ret = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    ASSERT_EQ(ret, MQTTCLIENT_SUCCESS);

    printf("Message with delivery token %d delivered\n", token);
    ret = MQTTClient_disconnect(client, 10000);
    ASSERT_EQ(ret, MQTTCLIENT_SUCCESS);

    MQTTClient_destroy(&client);
}
