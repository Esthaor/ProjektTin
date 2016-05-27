//
// Created by Michal Lipinski on 02.05.16.
//

#include "../include/Agent.h"
#include "../include/Socket.h"

Agent::Agent() {
    this->packet_counter = 0;
}

Agent::~Agent() {

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

int Agent::sniff() {

    char* device;               // The device to sniff on
    char error_buffer[PCAP_ERRBUF_SIZE];    // Error string

    bpf_u_int32 mask;		    // netmask
    bpf_u_int32 net;		    // IP

    pcap_t *handle;             // Session handle
    struct bpf_program fp;		// The compiled filter
    char filter_exp[] = "host www.google.com";	// The filter expression
    struct pcap_pkthdr header;	// The header that pcap gives us
    const u_char *packet;		// The actual packet


    /* Define the device */
    device = pcap_lookupdev(error_buffer);
    if (device == NULL) {
        fprintf(stderr, "Couldn't find default device: %s\n", error_buffer);
        exit(1);
    }

    /* Find the properties for the device */
    if (pcap_lookupnet(device, &net, &mask, error_buffer) == -1) {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", device, error_buffer);
        net = 0;
        mask = 0;
    }

    printf("Device: %s\n", device);

    /* Open the session in promiscuous mode */
    handle = pcap_open_live(device, BUFSIZ, 1, -1, error_buffer);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", device, error_buffer);
        exit(1);
    }

    /* Compile and apply the filter */
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        exit(1);
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        exit(1);
    }

    /* Grab a packet */
    pcap_loop(handle, -1, this->callback, NULL);

    return 0;
}

void Agent::callback(u_char *useless, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    static int count = 1;
    fprintf(stdout,"%d, ",count);
    fflush(stdout);
    count++;
}