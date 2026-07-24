#pragma once

#include <string>
#include "MatchingEngine.h"

class CSVEngine {
    public:
        CSVEngine(MatchingEngine& engine);
        void replay(const std::string& filename);
    private:
        MatchingEngine& matchingEngine;

};