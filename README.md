# Lora-Mesh & Sensor Node

This repo is a combination of work done in Lora radio transmission technology using ra-02(433Mhz) & rfm-95(868Mhz) chip.

All the codes are tested and working on Arduino Nano.

To start with:

* Lora_sensor_node uses UART available on Arduino to get the data and then transmits through Lora.

* ESP gateway is a method to build cheap single channel gateway out of ESP-32 by connecting it with ttn network.

* Lora_mesh_esp handles the mesh networking between different nodes as its an power hungry task it's not advisable to use any 8 bit microcontroller for the purpose.

* other example houses different use cases like Lora mesh visualizer and Lora + GPS combo.

* In documents and images collection of necessary Lora instructions and also some photos.

  ![IMG_20190906_131628](C:\Users\Bhaskar\Downloads\IMG_20190906_131628.jpg)

* Research paper folder houses all the paper which I use as citation in my research paper titled  "Lorawan mesh v/s Other Radio mesh", They are pretty informative in terms of efficient protocols for transmission.