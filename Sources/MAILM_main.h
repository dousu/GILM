#ifndef MAILM_MAIN_H
#define MAILM_MAIN_H

#include <cassert>
#include <cfloat>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "Agent.h"
#include "Dictionary.h"
#include "Element.h"
#include "Knowledge.h"
#include "LogBox.h"
#include "MAILMParameters.h"
#include "MT19937.h"
#include "Prefices.h"
#include "Rule.h"
#include "XMLreader.h"
#include "LilypondOutput.h"

void output_data(std::string, std::string);

#endif /* MAILM_MAIN_H */
