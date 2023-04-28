typedef pair<IPAddress, uint16_t> IPPortPair;
//lan ip/port mapping to nat port
unordered_map<IPPortPair, uint16_t> ipport_to_port_map;
//nat port mapping to lan ip/port 
unordered_map<uint16_t, IPPortPair> port_to_ipport_map;
Inet nat;
static uint16_t unique_port = 0;//begining from some port
uint16_t generate_unique_port(){
  return unique_port++;
}
class IPAddress {
  string ip;
  public:
    IPAddress(string ipaddr){ ipaddr = ip; }
    uint32_t addr(){ //return ipaddr in uint32_t }
};
struct Packet {
  uint32_t src_ip;
  uint32_t dst_ip;
  uint16_t src_port;
  uint16_t dst_port;
  bool is_lan;
  boo1 is_net;
  uint8_t buffer[PKT_SIZE];
};
bool check_packet(Packet *p){
  //packet validation checks
  if(...){ 
  }
  //get src/dst ip/port
  IPAddress src_ip(p->src_ip);
  IPAddress dst_ip(p->dst_ip);
  uint16_t src_port = p->src_port;
  uint16_t dst_port = p->dst_port;
  
  if(p->is_lan){
    if(ipport_to_port_map.count(IPPortPair{src_ip, src_port})==0){
      //record new path and assign a port on nat
      uint16_t new_port = generate_unique_port();
      ipport_to_port_map.insert(IPPortPair{src_ip, src_port}, new_port);
      port_to_ipport_map.insert(new_port, IPPortPair{src_ip, src_port});
    }else{
      //replace lan ip/port with nat ip/port in src
      p->src_ip = nat->ip.addr();
      p->src_port = ipport_to_port_map[IPPortPair{src_ip, src_port}];
    }
    return true;
  }else if(p->is_net){
    //check if a local nat mapping present for the dst_port
    if(port_to_ipport_map.count(dst_port)!=0){
      IPPortPair ippair = port_to_ipport_map[dst_port];
      //replace dst nat IP/port with actual dst found in nat mapping
      p->dst_ip = ippair.first.addr();
      p->dst_port = ippair.second;
      return true;
    }
    return false;
  }
}
