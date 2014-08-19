n = NetAddr.new( "127.0.0.1", 9005 );

n.sendMsg( "/data", 100, 50, 75 );

// input data should be between 0 and 100, floating point

n.sendMsg( "/learn" ); // wait for repetition and learn
n.sendMsg( "/clean" ); // clean complete classifier

// messages sent from classifier
"/RepClassifier/mostLikelyGesture"
"/RepClassifier/distance"
"/RepClassifier/phase"
"/RepClassifier/repetition"
