# FT_MALCOLM

---

## Virtual Machine Setup

This project requires two virtual machines: one as the victim and one as Malcolm (the attacker).

- **Debian 12.11.0 Image:** [Download](https://cdimage.debian.org/debian-cd/current/amd64/iso-cd/debian-12.11.0-amd64-netinst.iso)
- **Virtualization:** Oracle VM VirtualBox is recommended.

**Tip:** Install Guest Additions for clipboard sharing and drag-and-drop functionality.

### Shared Clipboard Configuration

After creating both VMs, enable clipboard sharing:

- In VirtualBox, go to **Devices > Shared Clipboard > Bidirectional** for each VM.

### Root Access

To avoid modifying the sudoers file, switch to root:

```bash
su -
```

### Installing Dependencies (on Malcolm VM)

Update and install required packages:

```bash
sudo apt-get update && sudo apt-get upgrade
sudo apt-get install -y vim git net-tools iproute2 arping iputils-ping tcpdump
git clone https://github.com/ftTower/ft_malcom.git ft_malcolm
cd ft_malcolm
echo done
```

---

## VM Network Configuration

Configure both VMs to use DHCP for the NAT network:

```bash
sudo bash -c 'cat <<EOF > /etc/network/interfaces
# This file describes the network interfaces available on your system
# and how to activate them. For more information see interfaces(5).

source /etc/network/interfaces.d/*

# The loopback network interface
auto lo
iface lo inet loopback

# The primary network interface
auto enp0s3
iface enp0s3 inet dhcp
EOF'
echo "Network configuration replaced. Implementing DHCP for NAT network."
sudo systemctl restart networking
clear && ip a
```

### Creating a NAT Network

1. In VirtualBox, go to **File > Tools > Network Manager > NAT Networks** and create a new NAT network.  
    ![Screenshot of Vbox](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/Vbox_NAT_network.png)

2. For both VMs, go to **Machine > Settings > Network**, set "Attached to" as **NAT Network**, and select your created network.  
    ![Screenshot of VM](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/vm_network.png)

---

## Man-in-the-Middle Attack

### Usage

After configuring the VMs:

- **(Optional)** Use `nmap` to scan IPs in a real scenario.

#### Gather Network Information

On Malcolm VM:

```bash
ip -r
```
- Output: `default via <gateway_ip>`

![Malcolm ip -r output](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/ip_r_output_malcolm.png)

```bash
ip -a
```
- Output: `link/ether <Malcolm_MAC_Address>`

![Malcolm ip -a output](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/ip_a_output_malcolm.png)

On Victim VM:

```bash
ip -a
```
- Output: `inet <Victim_IP>`
- Output: `link/ether <Victim_MAC_Address>`

![Victim ip -a output](https://github.com/ftTower/ftTower/blob/main/assets/Malcolm/ip_a_output_victim.png)

#### Run FT_MALCOLM

```bash
./ft_malcolm <gateway_ip> <malcolm_mac_address> <victim_ip> <victim_mac_address>
```

- `<gateway_ip>`: Found with `ip -r` on Malcolm VM.
- `<malcolm_mac_address>`: Found with `ip -a` on Malcolm VM.
- `<victim_ip>` and `<victim_mac_address>`: Found with `ip -a` on Victim VM.

---

## ARP Cache Flush on Victim VM

**Recommended method (using `ip`):**

The `ip` command is the modern and more powerful replacement for `ifconfig` and `route`.

To clear all ARP cache entries (ensures a fresh ARP resolution for all communications):

```bash
ip -s -s neigh flush all
```

- `ip neigh`: Manages the neighbor table (ARP table for IPv4, NDP table for IPv6).
- `flush all`: Removes all entries.
- `-s -s` (optional): Shows statistics before and after the flush.

Then, try:

```bash
ping google.com
```

Malcolm should detect an ARP request from your victim VM to the gateway.


### verify if victim is arp poisened
to display arp tables in victim vm
```bash
ip neigh show
```
you should see the gateway ip and 

---

## How It Works

### Typical Ethernet Frame Content

- **Preamble and Start Frame Delimiter (SFD):** Used for clock synchronization between devices.
- **Destination MAC Address:** The physical address of the recipient's network card.
- **Source MAC Address:** The physical address of the sender's network card.
- **Type/Length (EtherType):** Indicates the type of upper-layer protocol encapsulated in the frame (e.g., 0x0800 for IPv4, 0x0806 for ARP). This field tells the network card how to interpret the frame's content.
- **Data (Payload):** The actual content, which can be an IP packet, an ARP message, etc. The minimum data size in an Ethernet frame is 46 bytes (padding bytes are added if the content is smaller).
- **Frame Check Sequence (FCS):** An error-detection code to ensure the frame was not corrupted during transmission.

### ARP Table (ARP Cache)

The ARP table (Address Resolution Protocol) is a mapping table stored in memory (a cache) on each device in the network (computers, routers, etc.). Its role is to map IP addresses (Layer 3 logical addresses) to MAC addresses (Layer 2 physical addresses).

**How the ARP table works:**

- **ARP Request:** If a device needs the MAC address for an IP address it does not know (not present in its ARP table), it sends an ARP request as a broadcast on the local network. This request is encapsulated in an Ethernet frame.
- **ARP Reply:** The device that owns the requested IP address responds with its MAC address. This reply is sent as a unicast frame (directly to the requester).
- **ARP Table Update:** The device that initiated the request adds the IP-MAC entry to its ARP table for future communications.

// struct ethhdr {
//     unsigned char   h_dest[ETH_ALEN];   /* Destination Host Address */
//     unsigned char   h_source[ETH_ALEN]; /* Source Host Address      */
//     unsigned short  h_proto;            /* Protocol type            */
// };

struct ether_arp {
    struct arphdr   ea_hdr;     /* ARP hrd & proto et al */
    u_char          arp_sha[ETH_ALEN];  /* Sender hardware address */
    u_char          arp_spa[4];         /* Sender protocol address */
    u_char          arp_tha[ETH_ALEN];  /* Target hardware address */
    u_char          arp_tpa[4];         /* Target protocol address */
};

struct arphdr {
    unsigned short  ar_hrd;     /* Format of hardware address   */
    unsigned short  ar_pro;     /* Format of protocol address   */
    unsigned char   ar_hln;     /* Length of hardware address   */
    unsigned char   ar_pln;     /* Length of protocol address   */
    unsigned short  ar_op;      /* ARP opcode (command)         */
};

### OSI Model

**Layer 1: Physical Layer**  
Role: The lowest layer, responsible for transmitting raw bits over the physical medium (cables, Wi-Fi, fiber optics).  
Analogy: Like the electrical cable itself or radio waves for Wi-Fi. Defines electrical, mechanical, and functional specifications for establishing, maintaining, and disabling physical connections.  
Examples: RJ45 connectors, volts, Hz, bits, radio waves, Ethernet cables.

**Layer 2: Data Link Layer**  
Role: Ensures error-free data transmission over a direct link (between two physically connected devices). Manages access to the physical medium and physical addressing.  
Analogy: Like the mail carrier who puts addresses on envelopes and ensures the envelope is ready to be sent on the correct street. Splits bits into "frames" and adds MAC addresses.  
Examples: MAC addresses, Ethernet (Ethernet frames), switches, PPP, ARP.

**Layer 3: Network Layer**  
Role: Manages logical addressing and routing of packets across different networks (from one machine to another, even if they are not on the same local network).  
Analogy: Like a GPS or map system that finds the best route for your mail to reach the correct city, even if it's far away. Uses IP addresses for this.  
Examples: IP addresses, routers, IP protocol.

**Layer 4: Transport Layer**  
Role: Provides reliable end-to-end communication between two applications. Segments data into pieces (segments), numbers them, and ensures they all arrive at the destination in the correct order. Also manages flow control and error detection/correction.  
Analogy: Like the postal service that ensures your entire package arrives, even if it was divided into several boxes. If a box is lost, it asks the sender to resend it.  
Examples: TCP (Transmission Control Protocol - reliable, with acknowledgment), UDP (User Datagram Protocol - fast, without acknowledgment), port numbers (80 for HTTP, 443 for HTTPS, etc.).

**Layer 5: Session Layer**  
Role: Establishes, manages, and terminates communication sessions between applications. Keeps track of who is talking to whom and maintains the connection.  
Analogy: Like a phone call connection: you dial a number, talk, then hang up. Manages the beginning and end of the conversation.  
Examples: RPC (Remote Procedure Call), NetBIOS.

**Layer 6: Presentation Layer**  
Role: Ensures data format compatibility between applications. Translates, compresses, and encrypts data so that information sent by one application is understandable by another.  
Analogy: Like a translator or encoder ensuring the language you write is understood by the recipient (e.g., converting text data to images, or encrypting/decrypting data).  
Examples: JPEG, ASCII, EBCDIC, SSL/TLS (also operates at other levels, but has functions here).

**Layer 7: Application Layer**  
Role: The layer closest to the user. Provides network services directly to software applications. This is where users interact with the network.  
Analogy: The application you use: your web browser, email client, instant messaging app. It's what you see and interact with directly.  
Examples: HTTP (for the web), FTP (for file transfer), SMTP (for emails), DNS (for domain name resolution).

