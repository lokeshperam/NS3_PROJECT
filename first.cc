#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

int main (int argc, char *argv[])
{
  // Enable logging
  LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

  // Create nodes
  NodeContainer nodes;
  nodes.Create(2);

  // Create point-to-point link
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute("DataRate", StringValue("3Mbps"));
  pointToPoint.SetChannelAttribute("Delay", StringValue("30ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install(nodes);

  // Install internet stack
  InternetStackHelper stack;
  stack.Install(nodes);

  // Assign IP addresses
  Ipv4AddressHelper address;
  address.SetBase("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign(devices);

  // Create UDP server application
  UdpEchoServerHelper server(9);
  ApplicationContainer serverApps = server.Install(nodes.Get(1));
  serverApps.Start(Seconds(1.0));
  serverApps.Stop(Seconds(10.0));

  // Create UDP client application
  UdpEchoClientHelper client(interfaces.GetAddress(1), 9);
  client.SetAttribute("MaxPackets", UintegerValue(5));
  client.SetAttribute("Interval", TimeValue(Seconds(10.0)));
  client.SetAttribute("PacketSize", UintegerValue(1480));

  ApplicationContainer clientApps = client.Install(nodes.Get(0));
  clientApps.Start(Seconds(1.0));
  clientApps.Stop(Seconds(10.0));

  // Enable PCAP tracing
  pointToPoint.EnablePcapAll("scratch/first");

  // Run simulation
  Simulator::Run();
  Simulator::Destroy();

  return 0;
}
