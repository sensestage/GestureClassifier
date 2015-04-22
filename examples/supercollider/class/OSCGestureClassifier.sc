OSCGestureClassifier {

	classvar <>path;

	var <addr;
	var <dataTask;
	var <oscFuncs;
	var <>deltaTime = 0.025;

	var <data;

	var <interval, <repetition, <detected, <index, <distance, <phase, <learned;
	var <learning, <recording, <templateSize;

	var <actions;

	*new{ |port=9005|
		^super.new.init( port );
	}

	init{ |port|
		actions = IdentityDictionary.new;
		addr = NetAddr.new( "127.0.0.1", port );
		this.initOSCFuncs;
	}

	initOSCFuncs {
		oscFuncs = [
			OSCFunc( { |msg|
				repetition = msg[1]; // is a gate for interval
				if ( actions.at( \repetition ).notNil ){
					actions.at( \repetition ).value( msg[1] );
				};
			}, "/OSCGestureClassifier/repetition", addr ),
			OSCFunc( { |msg|
				interval = msg[1];
				if ( actions.at( \interval ).notNil ){
					actions.at( \interval ).value( msg[1] );
				};
			}, "/OSCGestureClassifier/interval", addr ),
			OSCFunc( { |msg|
				repetition = msg[1]; // is a gate for interval
				interval = msg[2];
				if ( actions.at( \periodic ).notNil ){
					actions.at( \periodic ).value( msg[1], msg[2] );
				};
			}, "/OSCGestureClassifier/repetition/all", addr ),


			OSCFunc( { |msg|
				detected = msg[1]; // is a gate for next three parameters
				if ( actions.at( \detected ).notNil ){
					actions.at( \detected ).value( msg[1] );
				};
			}, "/OSCGestureClassifier/detected", addr ),
			OSCFunc( { |msg|
				index = msg[1]; // index of currently detected gesture
				if ( actions.at( \index ).notNil ){
					actions.at( \index ).value( msg[1] );
				};
			},"/OSCGestureClassifier/index", addr ),
			OSCFunc( { |msg|
				distance = msg[1]; // distance from currently detected gesture template (accuracy)
				if ( actions.at( \distance ).notNil ){
					actions.at( \distance ).value( msg[1] );
				};
			}, "/OSCGestureClassifier/distance", addr ),
			OSCFunc( { |msg|
				phase = msg[1]; // phase within current gesture
				if ( actions.at( \phase ).notNil ){
					actions.at( \phase ).value( msg[1] );
				};
			}, "/OSCGestureClassifier/phase", addr ),
			OSCFunc( { |msg|
				// msg.postln;
				// detected = msg[1]; // is a gate for next three parameters
				if ( actions.at( \gesture ).notNil ){
					actions.at( \gesture ).value( *(msg.copyToEnd(1)) );
				};
			}, "/OSCGestureClassifier/detected/all", addr ),

			OSCFunc( { |msg|
				learned = msg[1]; // should set the number of gestures learned, or a collection
				if ( actions.at( \learned ).notNil ){
					actions.at( \learned ).value( msg[1] );
				};
			}, "/OSCGestureClassifier/learned", addr ),

			OSCFunc( { |msg|
				learning = msg[1];
				recording = msg[2];
				templateSize = msg[3];
				if ( actions.at( \learning ).notNil ){
					actions.at( \learning ).value( msg[1], msg[2], msg[3] );
				};
			}, "/OSCGestureClassifier/learning", addr ),

			OSCFunc( { |msg|
				if ( actions.at( \update ).notNil ){
					actions.at( \update ).value( msg[1], msg[2], msg[3] );
				};
			}, "/OSCGestureClassifier/update", addr )
		];
	}

	start{ |runTerm=true|
		dataTask = TaskProxy( { loop{
			if ( data.notNil ){
				addr.sendMsg( *(["/data"] ++ data) );
		}; this.deltaTime.wait; } } );
		dataTask.play;

		if ( runTerm ){
			( path + NetAddr.langPort + addr.port ).runInTerminal;
		}{
			this.clear;
		}
	}

	stop{
		// osc gesture classifier needs a quit message
		addr.sendMsg( "/quit" );
		dataTask.stop;
	}

	data_{ |indata|
		data = indata;
	}

	// messages to the classifier:

	learn{
		addr.sendMsg( "/learn" );
	}

	learnGate{ |onoff|
		addr.sendMsg( "/learn/gate", onoff.binaryValue );
	}

	templateSize_{ |size|
		addr.sendMsg( "/minimum_template_size", size );
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

	putAction{ |type,action|
		actions.put( type, action );
	}

	removeAction{ |type|
		actions.removeAt( type );
	}
}