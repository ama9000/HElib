/*
 * JSONParser.h
 *
 *  Created on: Jul 8, 2015
 *      Author: ph
 */

#ifndef JSONPARSER_H_
#define JSONPARSER_H_

#include <string.h>
#include <vector>
#include <jansson.h>
#include "util.h"


using namespace std;
using namespace NTL;


class JSONParser {
public:
	JSONParser();
	virtual ~JSONParser();
	json_t *encodeZZXToJSON(const ZZX poly);
	json_t *encodeZZ_pXToJSON(const ZZ_pX poly);
	ZZX extractZZX(json_t *jsonData);
	ZZ_pX extractZZ_pX(json_t *jsonData);
	void addZZXToJSON(const string name, const ZZX poly);
	void addZZ_pXToJSON(const string name, const ZZ_pX poly);
	void addZZToJSON(const string name, const ZZ val);
	void addZZ_pToJSON(const string name, const ZZ_p val);
	void addINTToJSON(const string name, const int val);
	void addDOUBLEToJSON(const string name, const double val);
	void addHomomorphismKeysToJSON(json_t *hkeys_json);
	int readJSONINT(const string name);
	double readJSONDOUBLE(const string name);
	ZZ readJSONZZ(const string name);
	ZZ_p readJSONZZ_p(const string name);
	ZZX readJSONZZX(const string name);
	ZZ_pX readJSONZZ_pX(const string name);
	json_t *getHomomorphismKeysJSONObj();

	void dumpJSONTxt();
	void dumpJSONToFile(const string json_file_prefix);
	void loadJSONFromFile(const string json_filename);
	bool isLoadedFromFile();



private:
	json_t *_root;
	const string _record_folder = "records";
	const string _homomorphismKeys = "hKeys";
	bool _loaded_from_file=false;
};



#endif /* JSONPARSER_H_ */
