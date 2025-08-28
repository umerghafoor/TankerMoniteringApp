import paho.mqtt.client as mqtt
import ssl
import time
import os

# AWS IoT Core endpoint
AWS_IOT_ENDPOINT = "a3aap03cjx0go8-ats.iot.us-east-1.amazonaws.com"
PORT = 8883

# Certificate paths (download from AWS IoT console)
CA_PATH = "secret/AmazonRootCA1.pem"
CERT_PATH = "secret/1fccffa823e5aec071612de1eda2bffee97c13571825deacfcfb58f2705a5901-certificate.pem.crt"
KEY_PATH = "secret/1fccffa823e5aec071612de1eda2bffee97c13571825deacfcfb58f2705a5901-private.pem.key"

TOPIC = "TankerManagement/pub"

# Check if certificate files exist
for path, name in [(CA_PATH, "CA certificate"), (CERT_PATH, "Device certificate"), (KEY_PATH, "Private key")]:
    if not os.path.isfile(path):
        raise FileNotFoundError(f"{name} not found at path: {path}")

def on_connect(client, userdata, flags, rc):
    print("Connected with result code:", rc)
    client.publish(TOPIC, "Hello from Python test!")

def on_message(client, userdata, msg):
    print("Message received:", msg.payload.decode())

client = mqtt.Client(client_id="TankerManagement")
client.on_connect = on_connect
client.on_message = on_message

# Configure TLS/SSL
client.tls_set(ca_certs=CA_PATH,
               certfile=CERT_PATH,
               keyfile=KEY_PATH,
               cert_reqs=ssl.CERT_REQUIRED,
               tls_version=ssl.PROTOCOL_TLSv1_2,
               ciphers=None)

client.connect(AWS_IOT_ENDPOINT, PORT, keepalive=60)
client.loop_start()

while True:
    time.sleep(5)
    client.publish(TOPIC, "ESP32 connection test")
    input("Press Enter to continue...")
