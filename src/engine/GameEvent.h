namespace Engine {
	struct NextLogicFrame {
		float elapsedTimeMS;
		NextLogicFrame(float tm) : elapsedTimeMS(tm) {}
	};

	struct NextRenderFrame {
		float elapsedTimeMS;
		NextRenderFrame(float tm) : elapsedTimeMS(tm) {}
	};


}