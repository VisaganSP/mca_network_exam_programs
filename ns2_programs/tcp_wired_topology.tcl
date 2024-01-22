# tcp_topology.tcl
set numNodes 10

# Create a simulator object
set ns [new Simulator]

# Create nodes
for {set i 0} {$i < $numNodes} {incr i} {
    set node($i) [$ns node]
}

# Create a simple link between nodes
for {set i 0} {$i < $numNodes-1} {incr i} {
    $ns duplex-link $node($i) $node([expr $i+1]) 10Mb 10ms DropTail
}

# Set up a TCP connection between the first and last node
set tcp [new Agent/TCP]
$ns attach-agent $node(0) $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $node([expr $numNodes - 1]) $sink
$ns connect $tcp $sink

# Set up a connection between the TCP agent and sink
set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ns at 1.0 "$ftp start"

# Define simulation duration
$ns at 10.0 "$ns stop"

# Run the simulation
$ns run

