#Set the global variable

set ns [new Simulator]

#create the file for animation

set namf [open wired3.nam w]

$ns namtrace-all $namf

 

#create the file for tracing

set tracef [open wired3.tr w]

$ns trace-all $tracef

#creation of nodes using for loop

for {set i 0} {$i < 10} {incr i} {

     set node($i) [$ns node]

}

#establishing the duplex links between the nodes

$ns duplex-link $node(0) $node(1) 2mb 1ms DropTail

$ns duplex-link $node(2) $node(3) 1mb 10ms DropTail

$ns duplex-link $node(1) $node(6) 3mb 10ms DropTail

$ns duplex-link $node(4) $node(3) 2mb 50ms DropTail

$ns duplex-link $node(8) $node(1) 3mb 25ms DropTail

$ns duplex-link $node(6) $node(4) 1mb 30ms DropTail

$ns duplex-link $node(7) $node(5) 2mb 15ms DropTail

$ns duplex-link $node(8) $node(4) 20mb 4ms DropTail

$ns duplex-link $node(2) $node(5) 2mb 100ms DropTail

$ns duplex-link $node(0) $node(9) 12mb 10ms DropTail

$ns duplex-link $node(0) $node(3) 20mb 14ms DropTail

$ns duplex-link $node(9) $node(7) 20mb 4ms DropTail

#define the procedure for smtp traffic with two nodes as parameters

proc traffic_smtp {n0 n1} {

global ns

set smtpsource [new Agent/UDP]

set smtpsink [new Agent/UDP]

$ns attach-agent $n0 $smtpsource

$ns attach-agent $n1 $smtpsink

$ns connect $smtpsource $smtpsink

set smtp_traffic [new Application/Traffic/Exponential]

$smtp_traffic attach-agent $smtpsource

$smtp_traffic set burst_time_ 50ms

$smtp_traffic set idle_time_ 50ms

$smtp_traffic set rate_ 100k

$smtp_traffic set packetSize_ 100

$ns at 0.1 "$smtp_traffic start"

$ns at 1.0 "$smtp_traffic stop"

}

#call the procedure

traffic_smtp $node(0) $node(1)

#define the procedure for CBR traffic with two nodes as parameters

proc traffic_cbr {n2 n3} {

global ns

set cbrsource [new Agent/UDP]

set cbrsink [new Agent/Null]

$ns attach-agent $n2 $cbrsource

$ns attach-agent $n3 $cbrsink

$ns connect $cbrsource $cbrsink

set cbr_traffic [new Application/Traffic/CBR]

$cbr_traffic attach-agent $cbrsource

$cbr_traffic set interval_ 50

$cbr_traffic set packetSize_ 50

$ns at 1.0 "$cbr_traffic start"

$ns at 2.0 "$cbr_traffic stop"

}

#call the CBR Traffic procedure

traffic_cbr $node(2) $node(3)

#define the procedure for FTP traffic with two nodes as parameters

proc traffic_ftp {n1 n6} {

global ns

set ftpsource [new Agent/TCP]

set ftpsink [new Agent/TCPSink]

$ns attach-agent $n1 $ftpsource

$ns attach-agent $n6 $ftpsink

$ns connect $ftpsource $ftpsink

set ftp_traffic [new Application/FTP]

$ftp_traffic attach-agent $ftpsource

$ftp_traffic set interval_ 50

$ftp_traffic set packetSize_ 50

$ns at 2.0 "$ftp_traffic start"

$ns at 3.0 "$ftp_traffic stop"

}

#call the ftp traffic procedure

traffic_ftp $node(1) $node(6)

#define the procedure for Telnet traffic with two nodes as parameters

proc traffic_telnet {n8 n1} {

global ns

set telnetsource [new Agent/TCP]

set telnetsink [new Agent/TCPSink]

$ns attach-agent $n8 $telnetsource

$ns attach-agent $n1 $telnetsink

$ns connect $telnetsource $telnetsink

set telnet_traffic [new Application/Telnet]

$telnet_traffic attach-agent $telnetsource

$telnet_traffic set interval_ 0.005

$ns at 3.0 "$telnet_traffic start"

$ns at 4.0 "$telnet_traffic stop"

}

#call the telnet traffic

traffic_telnet $node(8) $node(1)

#Trace annotation during the animation. Theese string will be displayed in the NAM window

$ns at 0.1 "$ns trace-annotate \"Time: 0.1 SMTP Traffic from Node 0 to Node 1 ..\""

$ns at 1.0 "$ns trace-annotate \"Time: 1.0 CBR Traffic from Node 2 to Node 3 ..\""

$ns at 2.0 "$ns trace-annotate \"Time: 2.0 FTP Traffic from Node 1 to Node 6..\""

$ns at 3.0 "$ns trace-annotate \"Time: 3.0 Telnet Traffic from Node 8 to Node 1..\""

#call the procedure finish

$ns at 5.0 "finish"

#creation of finish procedure

proc finish {} {

     global ns namf tracef

     $ns flush-trace

     close $namf

     close $tracef

}

$ns run
