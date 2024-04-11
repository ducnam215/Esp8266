#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <AHT20.h>
#include <stdlib.h>

#define Out_Red 0
#define Out_Green 2

long lastMsg = 0;
float temperature = 0;
float humidity = 0;

// Khai báo biến `ssid` là một con trỏ kiểu char, chứa tên của mạng WiFi mà bạn muốn kết nối.
const char *ssid = "Samsung Galaxy Z Flip4";
// Khai báo biến `password` là một con trỏ kiểu char, chứa mật khẩu của mạng WiFi mà bạn muốn kết nối.
const char *password = "241810697";
// Khai báo biến `mqtt_server` là một con trỏ kiểu char, chứa địa chỉ của máy chủ MQTT mà bạn muốn kết nối.
const char *mqtt_server = "valianthead152.cloud.shiftr.io";
// Khai báo biến `mqtt_port` kiểu int, chứa cổng kết nối đến máy chủ MQTT.
const int mqtt_port = 1883;
// Khai báo biến `mqtt_username` là một con trỏ kiểu char, chứa tên người dùng dùng để xác thực kết nối MQTT.
const char *mqtt_username = "valianthead152";
// Khai báo biến `mqtt_password` là một con trỏ kiểu char, chứa mật khẩu dùng để xác thực kết nối MQTT.
// Lưu ý: Mật khẩu ở đây đã được ẩn đi vì lí do bảo mật.
const char *mqtt_password = "test123";

// Khởi tạo đối tượng espClient từ class WiFiClient, sử dụng để thiết lập kết nối WiFi.
WiFiClient espClient;
// Khởi tạo đối tượng client từ class PubSubClient, sử dụng để gửi và nhận tin nhắn MQTT.
// Đối tượng này được khởi tạo với espClient làm tham số, điều này cho phép nó sử dụng kết nối WiFi đã thiết lập.
PubSubClient client(espClient);

AHT20 aht20;

void setup_wifi()
{
    delay(10); // Dừng chương trình trong một khoảng thời gian ngắn (10 milliseconds). Điều này có thể giúp ổn định module WiFi trước khi bắt đầu quá trình kết nối.

    // Bắt đầu kết nối WiFi bằng cách in thông báo trên cổng Serial.
    Serial.println();               // In một dòng trống để tạo khoảng cách trong log.
    Serial.print("Connecting to "); // In thông điệp "Connecting to " để báo hiệu đang tiến hành kết nối WiFi.
    Serial.println(ssid);           // In tên mạng WiFi (SSID) mà bạn đang cố gắng kết nối.

    WiFi.begin(ssid, password); // Bắt đầu kết nối WiFi bằng cách sử dụng SSID và mật khẩu đã được cung cấp.

    while (WiFi.status() != WL_CONNECTED) // Kiểm tra xem thiết bị đã kết nối thành công với WiFi hay chưa.
    {
        delay(500);        // Nếu chưa kết nối, hãy chờ 500 milliseconds trước khi kiểm tra lại.
        Serial.print("."); // In dấu chấm "." lên cổng Serial để biểu thị quá trình chờ kết nối.
    }

    // Khi thiết bị đã kết nối thành công với WiFi:
    Serial.println("");               // In một dòng trống.
    Serial.println("WiFi connected"); // Thông báo "WiFi connected" qua cổng Serial.
    Serial.println("IP address: ");   // Thông báo sẽ in ra địa chỉ IP của thiết bị.
    Serial.println(WiFi.localIP());   // In địa chỉ IP của thiết bị trong mạng WiFi mà nó vừa kết nối.
}

void callback(char *topic, byte *message, unsigned int length)
{
    // In thông điệp nhận được ra cổng Serial để debug hoặc theo dõi.
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    // Vòng lặp chuyển đổi tin nhắn từ dạng byte sang String.
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);  // In từng ký tự của tin nhắn.
        messageTemp += (char)message[i]; // Thêm từng ký tự vào chuỗi messageTemp.
    }
    Serial.println(); // Kết thúc dòng hiện tại trên cổng Serial.

    // Kiểm tra chủ đề của tin nhắn và thực hiện hành động tương ứng.
    if (strcmp(topic, "esp8266/client1/led/red") == 0)
    {
        Serial.print("Changing Room lamp to ");
        if (messageTemp == "On")
        {
            digitalWrite(Out_Red, HIGH);
            Serial.print("On");
        }
        else if (messageTemp == "Off")
        {
            digitalWrite(Out_Red, LOW);
            Serial.print("Off");
        }
    }

    // Làm tương tự như trên cho LED xanh lá.
    if (strcmp(topic, "esp8266/client1/led/green") == 0)
    {
        Serial.print("Changing Room lamp to ");
        if (messageTemp == "On")
        {
            digitalWrite(Out_Green, HIGH);
            Serial.print("On");
        }
        else if (messageTemp == "Off")
        {
            digitalWrite(Out_Green, LOW);
            Serial.print("Off");
        }
    }
}

void reconnect()
{
    // Vòng lặp cho đến khi thiết bị được kết nối trở lại với MQTT broker.
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Tạo một ID client ngẫu nhiên để tránh xung đột khi kết nối.
        String clientId = "ESP8266_Client1";
        // clientId += String(random(0xffff), HEX); // ID được tạo dựa trên một giá trị ngẫu nhiên, giúp đảm bảo tính duy nhất.

        // Thử kết nối với MQTT broker sử dụng ID client, tên người dùng và mật khẩu.
        if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
        {
            Serial.println("connected");
            // Nếu kết nối thành công, đăng ký lại các chủ đề cần thiết.
            client.subscribe("esp8266/client1/led/red");
            client.subscribe("esp8266/client1/led/green");
        }
        else
        {
            // Nếu kết nối thất bại, in ra mã lỗi và thử lại sau 5 giây.
            Serial.print("failed, rc=");
            Serial.print(client.state()); // In ra trạng thái kết nối hiện tại để giúp xác định lỗi.
            Serial.println(" try again in 5 seconds");
            // Đợi 5 giây trước khi thử kết nối lại.
            delay(5000);
        }
    }
}

void setup()
{
    Serial.begin(9600); // Bắt đầu giao tiếp serial ở tốc độ baud rate 115200.

    pinMode(Out_Red, OUTPUT);
    pinMode(Out_Green, OUTPUT);

    digitalWrite(Out_Red, LOW);
    digitalWrite(Out_Green, LOW);

    Wire.begin(); // Join I2C bus

    // Check if the AHT20 will acknowledge
    if (aht20.begin() == false)
    {
        Serial.println("AHT20 not detected. Please check wiring. Freezing.");
        while (1)
            ;
    }
    Serial.println("AHT20 acknowledged.");

    setup_wifi(); // Thực hiện cấu hình kết nối WiFi.
    // Thiết lập thông tin máy chủ MQTT và gán hàm callback để xử lý tin nhắn đến.
    client.setServer(mqtt_server, mqtt_port); // Thiết lập máy chủ MQTT và cổng kết nối.
    client.setCallback(callback);             // Gán hàm callback để xử lý tin nhắn MQTT đến.
}

void loop()
{
    // Kiểm tra nếu thiết bị không kết nối với MQTT broker, thực hiện quá trình kết nối lại.
    if (!client.connected())
    {
        reconnect(); // Gọi hàm reconnect() để thực hiện kết nối lại với MQTT broker.
    }

    client.loop(); // Xử lý các gói tin MQTT đến và gửi đi trong thư viện PubSubClient.

    long now = millis(); // Lấy thời gian hiện tại (tính bằng milliseconds) từ khi khởi động.

    // Kiểm tra nếu đã đến thời điểm gửi tin nhắn MQTT tiếp theo (cách nhau 500ms).
    if (now - lastMsg > 500)
    {
        lastMsg = now; // Cập nhật thời điểm gửi tin nhắn lần cuối.
                       // Chuyển đổi các giá trị nhiệt độ và độ ẩm sang chuỗi ký tự để gửi qua MQTT.

        // If a new measurement is available
        if (aht20.available() == true)
        {
            // Get the new temperature and humidity value
            temperature = aht20.getTemperature();
            humidity = aht20.getHumidity();

            static char temperatureTemp[7];
            dtostrf(temperature, 4, 1, temperatureTemp); // Chuyển đổi nhiệt độ sang chuỗi ký tự.
            static char humidityTemp[7];
            dtostrf(humidity, 4, 1, humidityTemp); // Chuyển đổi độ ẩm sang chuỗi ký tự.

            // Gửi giá trị nhiệt độ và độ ẩm qua MQTT với các chủ đề tương ứng.
            client.publish("esp8266/client1/aht20/temperature", temperatureTemp);
            client.publish("esp8266/client1/aht20/humidity", humidityTemp);
        }
    }
}