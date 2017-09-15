/*
 * JSONParser.cpp
 *
 *  Created on: Jul 8, 2015
 *      Author: ph
 */

#include "JSONParser.h"

#include <iostream>
#include <ctime>
#include <fstream>
#include<sys/stat.h>
#include<sys/types.h>

#include "util.h"


JSONParser::JSONParser() {
	_root = json_object();
}

JSONParser::~JSONParser() {
	json_decref(_root);
}

void JSONParser::addZZXToJSON(const string name, const ZZX poly){
	if(!_loaded_from_file){
		json_t *tmp_json_arr = json_array();
		for(int i=0; i<deg(poly)+1; i++){
			json_t *tmp =json_string(Helper::ZZToString(coeff(poly, i)).c_str());
			json_array_append( tmp_json_arr,  tmp);
			json_decref(tmp);
		}

		json_object_set_new(_root, name.c_str(), tmp_json_arr);
	}
}

void JSONParser::addZZ_pXToJSON(const string name, const ZZ_pX poly){
	if(!_loaded_from_file){
		json_t *tmp_json_arr = json_array();
		for(int i=0; i<deg(poly)+1; i++){
			json_t *tmp =json_string(Helper::ZZ_pToString(coeff(poly, i)).c_str());
			json_array_append( tmp_json_arr,  tmp);
			json_decref(tmp);
		}

		json_object_set_new(_root, name.c_str(), tmp_json_arr);
	}
}

json_t *JSONParser::encodeZZXToJSON(const ZZX poly){
	json_t *tmp_json_arr = json_array();
	for(int i=0; i<deg(poly)+1; i++){
		json_t *tmp =json_string(Helper::ZZToString(coeff(poly, i)).c_str());
		json_array_append( tmp_json_arr,  tmp);
		json_decref(tmp);
	}
	return tmp_json_arr;
}

json_t *JSONParser::encodeZZ_pXToJSON(const ZZ_pX poly){
	json_t *tmp_json_arr = json_array();
	for(int i=0; i<deg(poly)+1; i++){
		json_t *tmp =json_string(Helper::ZZ_pToString(coeff(poly, i)).c_str());
		json_array_append( tmp_json_arr,  tmp);
		json_decref(tmp);
	}
	return tmp_json_arr;
}

void JSONParser::addZZToJSON(const string name, const ZZ val){
	if(!_loaded_from_file){
		//cout<<"zzToString(val):"<<ZZToString(val)<<endl;
		json_t * tmp=json_string(Helper::ZZToString(val).c_str());
		json_object_set_new(_root, name.c_str(), tmp);
	}
}

void JSONParser::addZZ_pToJSON(const string name, const ZZ_p val){
	if(!_loaded_from_file){
		//cout<<"ZZ_pToString(val):"<<ZZ_pToString(val)<<endl;
		json_t * tmp=json_string(Helper::ZZ_pToString(val).c_str());
		json_object_set_new(_root, name.c_str(), tmp);
	}
}

void JSONParser::addINTToJSON(const string name, const int val){
	if(!_loaded_from_file){
		json_t *tmp= json_integer(val);
		json_object_set_new(_root, name.c_str(), tmp);
	}
}

void JSONParser::addDOUBLEToJSON(const string name, const double val){
	if(!_loaded_from_file){
		json_t *tmp= json_real(val);
		json_object_set_new(_root, name.c_str(), tmp);
	}
}



void JSONParser::addHomomorphismKeysToJSON(json_t *hkeys_json){
	if(!_loaded_from_file){
		json_object_set_new(_root, _homomorphismKeys.c_str(), hkeys_json);
	}
}



void JSONParser::dumpJSONTxt(){
	cout << json_dumps(_root, 0);
}

void JSONParser::dumpJSONToFile(const string json_file_prefix){
	if(!_loaded_from_file){
		std::time_t current_timestamp = std::time(nullptr);
		ofstream jsonfile;

		mkdir(_record_folder.c_str(), 0777);

		int randInt=rand() % 10000;
		string filename = to_string(current_timestamp)+"_"+to_string(randInt)+"_"+json_file_prefix+".json";
		cout << "json filename:" << _record_folder+"/"+filename << endl;
		jsonfile.open (_record_folder+"/"+filename);

		// have to assign it to a buffer and free it after used, otherwise memory leak
		char *buf = json_dumps(_root, 0);
		jsonfile << buf;
		free(buf);
		jsonfile.close();
	}
}


void JSONParser::loadJSONFromFile(const string json_filename){
	json_error_t error;
	string filepath=json_filename;
	//_record_folder + "/" +
	//cout<< filepath << endl;

	string buf;
	ifstream myfile (filepath);
	if (myfile.is_open()){
		getline (myfile,buf);
		//cout<< buf << endl;
		_root = json_loads(buf.c_str(), 0, &error );
		myfile.close();
	} else
		cerr << "Unable to open file" <<endl;

	_loaded_from_file=true;
}

ZZ JSONParser::readJSONZZ(const string name){
	json_t *jsonData = json_object_get( _root, name.c_str() );
	//cout << "ZZ " << json_string_value(jsonData) << endl;
	return to_ZZ(json_string_value(jsonData));
}

ZZ_p JSONParser::readJSONZZ_p(const string name){
	json_t *jsonData = json_object_get( _root, name.c_str() );
	//cout << "ZZ_p " << json_string_value(jsonData) << endl;
	return conv<ZZ_p>(to_ZZ(json_string_value(jsonData)));
}

int JSONParser::readJSONINT(const string name){
	json_t *jsonData = json_object_get( _root, name.c_str() );
	//cout << "int " << json_integer_value(jsonData) << endl;
	return json_integer_value(jsonData);
}

double JSONParser::readJSONDOUBLE(const string name){
	json_t *jsonData = json_object_get( _root, name.c_str() );
	return json_number_value(jsonData);
}

ZZX JSONParser::extractZZX(json_t *jsonData){
	ZZX poly;
	if( json_is_array( jsonData ) )
	{
		const uint length = json_array_size( jsonData );
		for( uint i=0; i<length; ++i ) // Iterates over the sequence elements.
		{
			json_t *jsonObject = json_array_get( jsonData, i );
			SetCoeff(poly, i, ZZ(to_ZZ(json_string_value(jsonObject))));
			//cout << json_string_value(jsonObject) << " ";
		}
		//cout << endl;
	}
	return poly;
}

ZZ_pX JSONParser::extractZZ_pX(json_t *jsonData){
	return to_ZZ_pX(extractZZX(jsonData));
}

ZZX JSONParser::readJSONZZX(const string name){
	json_t *jsonData = json_object_get( _root, name.c_str() );
	return extractZZX(jsonData);
}

ZZ_pX JSONParser::readJSONZZ_pX(const string name){
	return to_ZZ_pX(readJSONZZX(name));
}


json_t *JSONParser::getHomomorphismKeysJSONObj(){
	return json_object_get( _root, _homomorphismKeys.c_str() );
}


bool JSONParser::isLoadedFromFile(){
	return _loaded_from_file;
}




//void JSONParser::Test(){
//	char *ret_strings = NULL;
//	char *ret_string = "AAABBBCCC";
//
//	json_t *root = json_object();
//	json_t *result_json_arr = json_array();
//
//	json_object_set_new( root, "command", json_string( "link_status" ) );
//	json_object_set_new( root, "result", result_json_arr );
//
//	json_array_append( result_json_arr, json_string( "test" ) );
//	json_array_append( result_json_arr, json_string( "fail" ) );
//	ret_strings = json_dumps( root, 0 );
//	json_decref( root );
//
//	std::cout << ret_strings;
//	//return ret_strings;
//}


