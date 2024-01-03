#pragma once
#include<bits/stdc++.h>
#include "Colony.h"

class pickyAnt : public Colony{
	public: 
		using Colony::Colony;
		virtual void ant() override;
		
		// this is just for testing, but don't delete
		// virtual void antColonyFinishEvent() override;
};
