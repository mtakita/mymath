#pragma once

#include <vector>
#include <iostream>

using namespace std;

// Constraint values.
#define MINUS_ZERO_AND_PLUS_ZERO_SAME   0x0001
#define ROUND_TO_3DECIMAL_PLACES        0x0002
#define ROUND_TO_INTEGER                0x0004
#define PRINT_ANYWAY                    0x0008


#define ConstraintApplied( i ) ( i & constraintValue ) 

class Validator
{
public:

	void set(int constraint_value ) {

		constraintValue = constraintValue | constraint_value;
	}

	void add( int inTestNo, string inTestName, vector<float> inValue, vector<float> inAns_value) {

		char buff[256];
		sprintf_s(buff, "%d %s", inTestNo, inTestName.c_str() );
		string testName{ buff };

		testNames.push_back(testName);
		values.push_back(inValue);
		ans_values.push_back(inAns_value);
	}

	void validate() {

		ret_testNames.clear();
		ret_values.clear();
		ret_ans_values.clear();

		int numOfValues = values.size();
		if (numOfValues != ans_values.size()) {
			cerr << "ERROR.  The number of values and the number of ans_values are different." << endl;
			return;
		}

		for (int i = 0; i < numOfValues; i++) {

			string testName = testNames[i];

			vector<float> sampleVal = values[i];
			vector<float> sampleAnsVal = ans_values[i];

			int numOfSampleValues = sampleVal.size();
			if (numOfSampleValues != sampleAnsVal.size()) {
				cerr << "ERROR.  The number of sample values and the number of sample ans_values are different." << endl;
				return;
			}
/*
			vector<float> ret_val;
			vector<float> ret_ans_val;
			ret_val.clear();
			ret_ans_val.clear();
			*/
			bool resultMatch = true;
			for ( int j = 0; j < numOfSampleValues; j++) {

				float val = sampleVal[j];
				float ans_val = sampleAnsVal[j];

				if (ConstraintApplied(ROUND_TO_3DECIMAL_PLACES)) {
					val = roundf(val * 1000.0f) / 1000.0f;
					ans_val = roundf(ans_val * 1000.0f) / 1000.0f;
				}
				if (ConstraintApplied(ROUND_TO_INTEGER)) {
					val = roundf(val);
					ans_val = roundf(ans_val);
				}
				if (ConstraintApplied(MINUS_ZERO_AND_PLUS_ZERO_SAME)) {
					if (-0.0f == val) {
						val = fabs(val);
					}
					if (-0.0f == ans_val) {
						ans_val = fabs(ans_val);
					}
				}

				if (val != ans_val || ConstraintApplied(PRINT_ANYWAY)) {

					resultMatch = false;
				}
			}

			if (resultMatch == false) {
				ret_testNames.push_back(testName);
				ret_values.push_back(sampleVal);
				ret_ans_values.push_back(sampleAnsVal);
			}
		}

		int numOfRetValues = ret_values.size();

		for (int i = 0; i < numOfRetValues; i++) {

			string testName = ret_testNames[i];
			vector<float> ret_val = ret_values[i];
			vector<float> ret_ans_val = ret_ans_values[i];

			char buff[256];
			sprintf_s(buff, "%s", testName.c_str());
			cerr << buff << endl;

			if (ret_val.size() == 3) {
				sprintf_s(buff, " %7.9f %7.9f %7.9f", ret_val[0], ret_val[1], ret_val[2]);
				cerr << buff << endl;
				sprintf_s(buff, " %7.9f %7.9f %7.9f", ret_ans_val[0], ret_ans_val[1], ret_ans_val[2]);
				cerr << buff << endl;
			}
			else if (ret_val.size() == 9) {
				sprintf_s(buff, " %7.9f %7.9f %7.9f %7.9f %7.9f %7.9f %7.9f %7.9f %7.9f", ret_val[0], ret_val[1], ret_val[2], ret_val[3], ret_val[4], ret_val[5], ret_val[6], ret_val[7], ret_val[8]);
				cerr << buff << endl;
				sprintf_s(buff, " %7.9f %7.9f %7.9f %7.9f %7.9f %7.9f %7.9f %7.9f %7.9f", ret_ans_val[0], ret_ans_val[1], ret_ans_val[2], ret_ans_val[3], ret_ans_val[4], ret_ans_val[5], ret_ans_val[6], ret_ans_val[7], ret_ans_val[8]);
				cerr << buff << endl;
			}
			else if (ret_val.size() == 4) {
				sprintf_s(buff, " %7.9f %7.9f %7.9f %7.9f", ret_val[0], ret_val[1], ret_val[2], ret_val[3]);
				cerr << buff << endl;
				sprintf_s(buff, " %7.9f %7.9f %7.9f %7.9f", ret_ans_val[0], ret_ans_val[1], ret_ans_val[2], ret_ans_val[3]);
				cerr << buff << endl;
			}

		}
	}


	int constraintValue = 0;

	vector<string> testNames;
	vector<vector<float>> values;
	vector<vector<float>> ans_values;

	vector<string> ret_testNames;
	vector<vector<float>> ret_values;
	vector<vector<float>> ret_ans_values;

};

