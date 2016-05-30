//
// Created by Michal Lipinski on 02.05.16.
//

#include "../include/Agent.h"
#include "../include/Socket.h"

Agent::Agent(){
    //this->instance = this;
    this->port = -1;
    this->end_condition_value = -1;
    this->packet_counter = 0;
}

Agent::Agent(unsigned capture_id, string port, EndCondition end_condition, int end_condition_value, int alarm) {
    //start_sniffing_time = NULL;
    thread_timeout = NULL;
    thread_alarm = NULL;

    this->capture_id = capture_id;

    if(port == "port ")
        this->port += "port 80";
    else
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
    this->all_captured_length = 0;
    this->all_total_length = 0;

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
    std::cout << "all captured packets: " << this->packet_counter << std::endl;
    std::cout << "all captured packets length: " << this->all_captured_length << std::endl;
    std::cout << "all packets length: " << this->all_total_length << std::endl;

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

    pcap_freealldevs(all_devices_list);

    std::cout << "End of list" << endl;
    return 0;
}

char* Agent::findDevice() {
    pcap_if_t* all_devices_list;
    pcap_if_t* iterator;
    char error_buffer[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&all_devices_list, error_buffer) == -1) {
        fprintf(stderr, "There is a problem with pcap_findalldevs: %s\n", error_buffer);
        return NULL;
    }

    for (iterator = all_devices_list; iterator != NULL; iterator = iterator->next) {
        for (pcap_addr_t* device_address = iterator->addresses; device_address != NULL; device_address = device_address->next) {
            if (device_address->addr->sa_family == AF_INET && device_address->addr && device_address->netmask) {
                return iterator->name;
            }
        }
    }
}

int Agent::sniff() {

    char* device;               // The device to sniff on

    char error_buffer[PCAP_ERRBUF_SIZE];    // Error string

    bpf_u_int32 netmask;		    // netmask
    bpf_u_int32 ip;		    // IP

    struct bpf_program fp;		// The compiled filter
    const char* filter_exp = this->port.c_str();	// The filter expression

    boost::thread(boost::bind(Socket::sendToServer, this->buildJson("started")));

    device = this->findDevice();

    /* Find the properties for the device */
    if(pcap_lookupnet(device, &ip, &netmask, error_buffer) == -1) {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", device, error_buffer);
        ip = 0;
        netmask = 0;
    }

    printf("Device: %s\n", device);

    /* Open the session in promiscuous mode */
    this->handler = pcap_open_live(device, BUFSIZ, 1, 1000, error_buffer);
    if(handler == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", device, error_buffer);
        exit(2);
    }

    /* Compile and apply the filter */
    if(pcap_compile(this->handler, &fp, filter_exp, 0, ip) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handler));
        exit(3);
    }

    if(pcap_setfilter(this->handler, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handler));
        exit(4);
    }

    /* Grab packets */
    try {
        if (this->end_condition == PACKETS) {
            if (this->enable_alarms)
                this->thread_alarm = new boost::thread(boost::bind(&Agent::signalAlarm, this));
            pcap_loop(this->handler, this->end_condition_value, this->callback, (u_char *) this);
        }
        else {
            this->start_sniffing_time = time(NULL);
            if (this->enable_alarms)
                this->thread_alarm = new boost::thread(boost::bind(&Agent::signalAlarm, this));
            this->thread_timeout = new boost::thread(boost::bind(&Agent::stopSniffing, this));
            pcap_loop(this->handler, -1, this->callback, (u_char *) this);
        }
    }
    catch(boost::thread_interrupted&){
        cout << "ZŁAPANY!" << endl;
        pcap_breakloop(this->handler);
    }

    Socket::sendToServer(this->buildJson("results"));

    return 0;
}

void Agent::callback(u_char *args, const struct pcap_pkthdr *packet_header, const u_char *packet_body) {
        Agent *instance = (Agent *) args;
        instance->packetInfo(packet_body, *packet_header);
        boost::this_thread::interruption_point();
}

void Agent::packetInfo(const u_char *packet_body, struct pcap_pkthdr packet_header) {
    //try {
        this->packet_counter++;
        this->all_captured_length += (int) packet_header.caplen; //amount of data available
        this->all_total_length += (int) packet_header.len; //actual length of packet
        /*boost::this_thread::interruption_point();
    }
    catch(boost::thread_interrupted&){
        cout << "ZŁAPANY!" << endl;
        pcap_breakloop(this->handler);
    }
    */
}

void Agent::signalAlarm() {
    while(true){
        sleep(1);
        if(this->packet_counter >= this->alarm){
            std::cout << "Breaking pcap_loop: alarm" << std::endl;
            Socket::sendToServer(this->buildJson("alarm"));
            break;
        }
    }
}

void Agent::stopSniffing() {
    while(true){
        sleep(1);
        if(difftime(time(NULL), this->start_sniffing_time) >= (double)this->end_condition_value) {
            std::cout << "Breaking pcap_loop: timeout" << std::endl;
            pcap_breakloop(this->handler);
            break;
        }
    }
}

string Agent::buildJson(string type) {
    ptree contents;

    contents.put("status", type);
    contents.put("id", this->capture_id);
    contents.put("port", this->port.substr(5));
    contents.put("alarmValue", this->alarm);
    contents.put("currentValue", this->all_total_length);
    contents.put("datetime", std::time(NULL));

    std::ostringstream buf;
    write_json(buf, contents, false);
    string json = buf.str();
    std::cout << "json " << type << ":\n" << json << std::endl;
    return json;
}