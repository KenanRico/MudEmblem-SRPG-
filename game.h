

#ifndef GAME_H
#define GAME_H

namespace Game{
	class State{
		public:
			enum STATE{ RUN=0, PAUSE=1, END=2 };
	};
	class Event{
		public:
			enum POSITION{ X=0, Y=1 };
			enum BUTTON{ LEFT=0, RIGHT=1 };
	};



};

#endif