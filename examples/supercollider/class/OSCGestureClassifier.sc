OSCGestureClassifier {

	classvar <>path;

	var <addr;
	var <dataTask;
	var <oscFuncs;
	var <>deltaTime = 0.025;

	var <data;

	var <interval, <repetition, <detected, <index, <distance, <phase, <learned;

	*new{ |port=9005|
		^super.new.init( port );
	}

	init{ |port|
		addr = NetAddr.new( "127.0.0.1", port );
		this.initOSCFuncs;
	}

	initOSCFuncs {
		oscFuncs = [
			OSCFunc( { |msg|
				repetition = msg[1]; // is a gate for interval
			}, "/OSCGestureClassifier/repetition" ),
			OSCFunc( { |msg|
				interval = msg[1];
			}, "/OSCGestureClassifier/interval" ),


			OSCFunc( { |msg|
				detected = msg[1]; // is a gate for next three parameters
			}, "/OSCGestureClassifier/detected" ),
			OSCFunc( { |msg|
				index = msg[1]; // index of currently detected gesture
			},"/OSCGestureClassifier/index" ),
			OSCFunc( { |msg|
				distance = msg[1]; // distance from currently detected gesture template (accuracy)
			}, "/OSCGestureClassifier/distance" ),
			OSCFunc( { |msg|
				phase = msg[1]; // phase within current gesture
			}, "/OSCGestureClassifier/phase" ),

			OSCFunc( { |msg|
				learned = msg[1]; // should set the number of gestures learned, or a collection
			}, "/OSCGestureClassifier/learned" )
		];
	}

	start{
		dataTask = TaskProxy( { loop{
			if ( data.notNil ){
				addr.sendMsg( *(["/data"] ++ data) );
		}; this.deltaTime.wait; } } );
		dataTask.play;

		( path + NetAddr.langPort + addr.port ).runInTerminal;
	}

	stop{
		// osc gesture classifier needs a quit message
		dataTask.stop;
	}

	data_{ |indata|
		data = indata;
	}

	// messages to the classifier:

	learn{
		addr.sendMsg( "/learn" );
	}

	recognize{
		addr.sendMsg( "/recognize" );
	}

	clear{
		addr.sendMsg( "/clear" );
	}

	threshold_{ |val|
		addr.sendMsg( "/threshold", val );
	}

}