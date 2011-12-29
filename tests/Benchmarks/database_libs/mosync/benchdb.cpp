/*
 * benchdb.cpp
 *
 *  Created on: 3 nov 2011
 *      Author: Alex
 *
 *  Uses an environment and a HttpConnectionListener to send HTTP/GET to URL (which should point to a publish script)
 *	TODO: Get device info in this library instead of having to send that as parameters to the functions
 *
 */

#include "benchdb.h"

BenchDBConnector::BenchDBConnector(BenchResult& br) : mHttp(this) //constructor, taking the complete url of the publish_script as an arg
, mIsConnected(false)
{

	char devProperties[200];
	char devPropertiesUrlEnc[200];
	char completeUrl[1000];

	int ret = maGetSystemProperty("mosync.device", devProperties, 200); //store the properties of the device in the buffer devProperty
	printf("Device properties: %s, retval: %d\n", devProperties, ret);

	//remove whitespaces, and use a placeholder device name if maGetSystemProperty fails
	if(ret < 0){
		br.phone = "ThisIsATestPhone";
	}else{
		for(int i = 0, j = 0; i < strlen(devProperties); ++i){
			if(devProperties[i] == ' '){
				//do nothing
			}else{
				devPropertiesUrlEnc[j] = devProperties[i];
				++j;
			}
		}
		br.phone = devPropertiesUrlEnc;
	}

	br.git_hash = "789";
	br.nativeSdkVer = "0";
	br.revision = "0";
	br.runtime = "MoSync";

	printf("Publishing result via url:");

	if(strcmp(br.benchmark, "linpack") == 0)
		sprintf(completeUrl, "%s%s%s%s%s%s%s%s%s%s%s%s%.3f", M_URL, "?benchmark=linpack&revision=", br.revision, "&runtime=", br.runtime, "&git_hash=", br.git_hash, "&phone=", br.phone, "&native_sdk_ver=", br.nativeSdkVer, "&mflops=", br.mflops);

	printf(completeUrl);
	initiateConnection(completeUrl); //connect to the url, publishing the results via HTTP GET
}
// connect to the given url if not other connection is active
void BenchDBConnector::initiateConnection(const char* url) {
	if(mIsConnected) {
		printf("already connected\n..");
		return;
	}
	printf("\nconnecting to %s", url);

	int res = mHttp.create(url, HTTP_GET); //we publish using HTTP GET
	if(res < 0) {
		printf("unable to connect - %i\n", res);
	} else {
		mHttp.finish();
		mIsConnected = true;
	}
}
void BenchDBConnector::httpFinished(MAUtil::HttpConnection* http, int result) {
	printf("HTTP %i\n", result);

	MAUtil::String contentLengthStr;
	int responseBytes = mHttp.getResponseHeader("content-length", &contentLengthStr);
	int contentLength = 0;
	if(responseBytes == CONNERR_NOHEADER)
		printf("no content-length response header\n");
	else {
		printf("content-length : %s\n", contentLengthStr.c_str());
		contentLength = atoi(contentLengthStr.c_str());
	}
	if(contentLength >= CONNECTION_BUFFER_SIZE || contentLength == 0) {
		printf("Receive in chunks..\n");
		mHttp.recv(mBuffer, CONNECTION_BUFFER_SIZE);
	} else {
		mBuffer[contentLength] = 0;
		mHttp.read(mBuffer, contentLength);
	}

}
void BenchDBConnector::connReadFinished(MAUtil::Connection* conn, int result) {
	if(result >= 0)
		printf("connReadFinished %i\n", result);
	else
		printf("connection error %i\n", result);
	mHttp.close();

	mIsConnected = false;
}
void BenchDBConnector::connRecvFinished(MAUtil::Connection * conn, int result){
	if(result >= 0) {
		printf("connRecvFinished %i\n", result);
		mHttp.recv(mBuffer, CONNECTION_BUFFER_SIZE);
		return;
	}
	else if(result == CONNERR_CLOSED) {
		printf("Receive finished!\n");
	} else {
		printf("connection error %i\n", result);
	}
	mHttp.close();
	mIsConnected = false;
}

/*
 * Publish a linpack result to the URL defined at the top of this file
 * revision = mosync revision
 * runtime = runtime (Mosync on Android/Mosync on iOS etc)
 * git_hash = the git_hash of the mosync build
 * phone = phone (Samsung Galaxy S etc.)
 * native_sdk_ver = the version of the native sdk
 * mflops = the number of mega flops
 */
//extern "C" int publish_linpack_result(char * revision, char * runtime, char * git_hash, char * phone, char * native_sdk_ver, float mflops) {
//
//	InitConsole();
//	gConsoleLogging = 1;
//
//	char complete_url[1000]; //build and store our url here
//
//	/* building the url using sprintf */
//	sprintf(complete_url, "%s%s%s%s%s%s%s%s%s%s%s%s%.3f", URL, "?benchmark=linpack&revision=", revision, "&runtime=", runtime, "&git_hash=", git_hash, "&phone=", phone, "&native_sdk_ver=", native_sdk_ver, "&mflops=", mflops);
//
//	BenchDBConnector * cm = new BenchDBConnector(complete_url); //pass the url to the ConnectionMoblet
//	//MAUtil::Moblet::run(cm); //run it
//	delete cm;
//	return 0;
//
//}
//
///*
// * Publish a opengles result to the URL defined at the top of this file
// * revision = mosync revision
// * runtime = runtime (Mosync on Android/Mosync on iOS etc)
// * git_hash = the git_hash of the mosync build
// * phone = phone (Samsung Galaxy S etc.)
// * native_sdk_ver = the version of the native sdk
// * test1/2/3/4 = the result of the test #
// */
//extern "C" int publish_opengl_result(char * revision, char * runtime, char * git_hash, char * phone, char * native_sdk_ver, int test1, int test2, int test3, int test4) {
//
//	InitConsole();
//	gConsoleLogging = 1;
//
//	char complete_url[1000]; //build and store our url here
//
//	/* building the url using sprintf */
//	sprintf(complete_url, "%s%s%s%s%s%s%s%s%s%s%s%s%d%s%d%s%d%s%d", URL, "?benchmark=opengl&revision=", revision, "&runtime=", runtime, "&git_hash=", git_hash, "&phone=", phone, "&native_sdk_ver=", native_sdk_ver, "&test1=", test1, "&test2=", test2, "&test3=", test3, "&test4=", test4);
//
//	BenchDBConnector * cm = new BenchDBConnector(complete_url); //pass the url to the ConnectionMoblet
//	//MAUtil::Moblet::run(cm); //run it
//	delete cm;
//	return 0;
//
//}
//
///*
// * Publish a membench result to the URL defined at the top of this file
// * revision = mosync revision
// * runtime = runtime (Mosync on Android/Mosync on iOS etc)
// * git_hash = the git_hash of the mosync build
// * phone = phone (Samsung Galaxy S etc.)
// * native_sdk_ver = the version of the native sdk
// */
//extern "C" int publish_membench_result(char * revision, char * runtime, char * git_hash, char * phone,
//		char * native_sdk_ver, float alloc_str_10, float alloc_str_100, float alloc_void_1, float alloc_void_100,
//		float alloc_void_1000, float alloc_dummy, float alloc_dummy_struct, float alloc_dummy_mix,
//		float access_array, float access_vector, float add_vector, float access_dummy, float access_dummy_struct,
//		float access_dummy_mix) {
//
//	InitConsole();
//	gConsoleLogging = 1;
//
//	char complete_url[1000]; //build and store our url here
//
//	/* building the url using sprintf TODO fix all parameters (if the sprintf-call is set up wrong we will get data oob in the caller of this lib function) */
//	sprintf(complete_url, "%s%s%s%s%s%s%s%s%s%s%s%s%f%s%f%s%f%s%f%s%f%s%f%s%f%s%f%s%f%s%f%s%f%s%f%s%f%s%f", URL, "?benchmark=membench&revision=", revision, "&runtime=", runtime, "&git_hash=",
//			git_hash, "&phone=", phone, "&native_sdk_ver=", native_sdk_ver, "&alloc_str_10=", alloc_str_10, "&alloc_str_100=", alloc_str_100,
//			"&alloc_void_1=", alloc_void_1, "&alloc_void_100=", alloc_void_100, "&alloc_void_1000=", alloc_void_1000,
//			"&alloc_dummy=", alloc_dummy, "&alloc_dummy_struct=", alloc_dummy_struct, "&alloc_dummy_mix=", alloc_dummy_mix, "&access_array=", access_array,
//			"&access_vector=", access_vector, "&add_vector=", add_vector, "&access_dummy=", access_dummy,
//			"&access_dummy_struct=", access_dummy_struct, "&access_dummy_mix=", access_dummy_mix);
//
//	BenchDBConnector * cm = new BenchDBConnector(complete_url); //pass the url to the ConnectionMoblet
//	//MAUtil::Moblet::run(cm); //run it
//	delete cm;
//	return 0;
//
//}


