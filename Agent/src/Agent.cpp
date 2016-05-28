//
// Created by Michal Lipinski on 02.05.16.
//

#include "../include/Agent.h"
#include "../include/Socket.h"



Agent::Agent(){
    this->port = -1;
    this->end_condition_value = -1;
    this->packet_counter = 0;
}

Agent::Agent(int port, EndCondition end_condition, int end_condition_value, int alarm) {
    this->port = port;

    if(end_condition == NONE) {
        this->end_condition = TIME;
        this->end_condition_value = 60;
    }
    else {
        this->end_condition = end_condition;
        this->end_condition_value = end_condition_value;
    }

    if(alarm == -1) {
        this->enable_alarms = false;
        this->alarm = -1;
    }
    else {
        this->enable_alarms = true;
        this->alarm = alarm;
    }

    this->packet_counter = 0;
}

Agent::~Agent() {

}

void Agent::displayInformation() {
    std::cout << "port: " << this->port << std::endl;
    switch(this->end_condition){
        case PACKETS:
            std::cout << "end_condition: PACKETS " << std::endl;
            break;
        case TIME:
            std::cout << "end_condition: TIME " << std::endl;
            break;
        case NONE:
            std::cout << "end_condition: NONE " << std::endl;
            break;
    }
    std::cout << "end_condition_value: " << this->end_condition_value << std::endl;
    std::cout << "alarm: " << this->alarm << std::endl;

}

bool Agent::receiveJson() {

    int port = 5010;

    Socket* socket = new Socket();
    socket->configureSocket(port);
    socket->startCommunication();

   /* // Przykładowy JSON
    string json = "{\"measurement\":{\"status\":\"start\",\"port\":\"22\",\"endCondition\":\"threshold\",\"endConditionValue\":\"666\",\"alarmValue\":\"0\"}}";

    ptree root;
    istringstream is (json);
    read_json (is, root); //konwersja do ptree
    cout << root.get<string>("measurement.status") << endl; //odczyt przykładowego elementu*/


    return true;
}

int Agent::showAllDevices() {
    pcap_if_t* all_devices_list;
    pcap_if_t* iterator;
    char error_buffer[PCAP_ERRBUF_SIZE];

    if(pcap_findalldevs(&all_devices_list, error_buffer) == -1){
        fprintf(stderr, "Couldn't find all devices: %s\n", error_buffer);
        exit(5);
    }

    if(all_devices_list == NULL) {
        std::cout << "No devices found" << std::endl;
        return 0;
    }

    iterator = all_devices_list;

    std::cout << "Devices:" << std::endl;

    do{
        //printf("Name: %s\nAddress: %s\nDescription: %s\n", iterator->name, iterator->addresses->addr->sa_data, iterator->description);
        printf("Name: %s\nDescription: %s\n", iterator->name, iterator->description);
        iterator = iterator->next;
    }while(iterator->next != NULL);

    std::cout << "End of list" << endl;
    return 0;
}

int Agent::sniff() {

    char* device;               // The device to sniff on
    char error_buffer[PCAP_ERRBUF_SIZE];    // Error string

    bpf_u_int32 netmask;		    // netmask
    bpf_u_int32 ip;		    // IP

    pcap_t *handle;             // Session handle
    struct bpf_program fp;		// The compiled filter
    char filter_exp[] = "host www.onet.pl";	// The filter expression
    struct pcap_pkthdr header;	// The header that pcap gives us
    const u_char *packet;		// The actual packet
    u_char* args = NULL;


    /* Define the device */
    device = pcap_lookupdev(error_buffer);
    if(device == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", error_buffer);
        exit(1);
    }

    /* Find the properties for the device */
    if(pcap_lookupnet(device, &ip, &netmask, error_buffer) == -1) {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", device, error_buffer);
        ip = 0;
        netmask = 0;
    }

    printf("Device: %s\n", device);

    /* Open the session in promiscuous mode */
    /*if(this->end_condition == TIME)
        handle = pcap_open_live(device, BUFSIZ, 1, this->end_condition_value, error_buffer);
    else*/
    handle = pcap_open_live(device, BUFSIZ, 1, -1, error_buffer);
    if(handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", device, error_buffer);
        exit(2);
    }

    /* Compile and apply the filter */
    if(pcap_compile(handle, &fp, filter_exp, 0, ip) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        exit(3);
    }

    if(pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        exit(4);
    }

    /* Grab packets */
    if(this->end_condition == PACKETS)
        pcap_loop(handle, this->end_condition_value, this->callback, args);
    else
        pcap_loop(handle, -1, this->callback, args); //forever

    return 0;
}

void Agent::callback(u_char *args, const struct pcap_pkthdr *packet_header, const u_char *packet_body) {
    static int count = 1;
    fprintf(stdout,"%d, ",count);
    fflush(stdout);
    count++;

    //packet_info(packet_body, *packet_header);

    printf("packet capture length: %d\n", packet_header->caplen);
    printf("packet total length: %d\n", packet_header->len);

}

/*void Agent::packet_info(const u_char *packet_body, struct pcap_pkthdr packet_header) {

}*/