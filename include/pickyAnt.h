#pragma once
#include<bits/stdc++.h>
#include "Colony.h"

class pickyAnt : Colony{
	public: 
		using Colony::Colony;
		virtual void ant() override;
};
