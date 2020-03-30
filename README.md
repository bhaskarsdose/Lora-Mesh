# Lora-Mesh & Sensor Node

This repo is a combination of work done in Lora radio transmission technology using ra-02(433Mhz) & rfm-95(868Mhz) chip.

All the codes are tested and working on Arduino Nano.

To start with:

* Lora_sensor_node uses UART available on Arduino to get the data and then transmits through Lora.

  ![Ver_1_node](C:\Users\Bhaskar\Desktop\New folder\Lora-Mesh\Documents & Images\Ver_1_node.jpeg)

* ESP gateway is a method to build cheap single channel gateway out of ESP-32 by connecting it with ttn network.

  ![Sending_data](C:\Users\Bhaskar\Desktop\New folder\Lora-Mesh\Documents & Images\Sending_data.jpeg)

* Lora_mesh_esp handles the mesh networking between different nodes as its an power hungry task it's not advisable to use any 8 bit microcontroller for the purpose.

* other example houses different use cases like Lora mesh visualizer and Lora + GPS combo.

  ![Node_comp](C:\Users\Bhaskar\Desktop\New folder\Lora-Mesh\Documents & Images\Node_comp.jpeg)

* In documents and images collection of necessary Lora instructions and also some photos.

  ![Gateway_conn](C:\Users\Bhaskar\Desktop\New folder\Lora-Mesh\Documents & Images\Gateway_conn.jpeg)

* Research paper folder houses all the paper which I use as citation in my research paper titled  "Lorawan mesh v/s Other Radio mesh", They are pretty informative in terms of efficient protocols for transmission.