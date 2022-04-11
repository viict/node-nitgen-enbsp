# include <node.h>
# include <iostream>
# include <fstream>
# include <cstring>
# include <string>
# include "nbiobsp.h"

using namespace std;

namespace NBioBSP {

	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::String;
	using v8::Number;
	using v8::Boolean;
	using v8::Value;

	void InitModule(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();

		ret = NBioAPI_Init(&bspHandle);
		if (ret != NBioAPIERROR_NONE) {
			args.GetReturnValue().Set(Number::New(isolate, ret));
			return;
		}

		idDevice = NBioAPI_DEVICE_ID_AUTO; // Abrindo o dispositivo para uso.
		ret = NBioAPI_OpenDevice(bspHandle, idDevice);
		args.GetReturnValue().Set(Number::New(isolate, ret));
	}

	void SetSkin(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();

		if (args.Length() == 1) {
			v8::String::Utf8Value skinPathFromArgs(isolate, args[0]);
			std::string name = std::string(*skinPathFromArgs);
			ifstream ifile;
			std::string fileName = name.c_str();
			ifile.open(fileName.c_str());
			if(ifile) {
				ifile.close();
				NBioAPI_SetSkinResource(name.c_str());
				args.GetReturnValue().Set(Boolean::New(isolate, true));
				return;
			}
		}

		args.GetReturnValue().Set(Boolean::New(isolate, false));
	}

	// Captura do template FIR da digital.
	void Capture(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		double timeout = 5;
		NBioAPI_FIR_PURPOSE purpose = NBioAPI_FIR_PURPOSE_ENROLL;

		if (args.Length() == 1) {
			v8::String::Utf8Value nameFromArgs(isolate, args[0]);
			std::string name = std::string(*nameFromArgs);
			timeout = ::atof(name.c_str());
		} else if (args.Length() == 2) {
			v8::String::Utf8Value nameFromArgs(isolate, args[0]);
			std::string name = std::string(*nameFromArgs);
			timeout = ::atof(name.c_str());

			v8::String::Utf8Value purposeFromArgs(isolate, args[1]);
			std::string purposeString = std::string(*purposeFromArgs);
			purpose = ::atof(purposeString.c_str());
		} else {
			args.GetReturnValue().Set(Number::New(isolate, ERROR_MISSING_ARGS));
			return;
		}

		NBioAPI_FIR_HANDLE firHandle;
		// purpose = NBioAPI_FIR_PURPOSE_IDENTIFY; // NBioAPI_FIR_PURPOSE_ENROLL;
		ret = NBioAPI_Capture(
			bspHandle,   // Módulo para manusear módulo NBioBSP
			purpose,     // Propósito da captura
			&firHandle,  // Manipular o FIR capturado
			timeout,     // Timeout para captura
			NULL,        // Dados para auditoria
			NULL         // Opção da janela
		);

		// Capturar FIR da digital e retornar o hash do template.
		NBioAPI_FIR_TEXTENCODE textFIR;
		NBioAPI_GetTextFIRFromHandle(bspHandle, firHandle, &textFIR, NBioAPI_FALSE);

		if (ret == NBioAPIERROR_NONE) {
			v8::Local<v8::String> result;
			v8::MaybeLocal<v8::String> temp = String::NewFromUtf8(isolate, textFIR.TextFIR);
			temp.ToLocal(&result);
			args.GetReturnValue().Set(result);
		} else {
			args.GetReturnValue().Set(Number::New(isolate, ret)); 
		}
	}

	// Método para fazer match de digitais.
	// nbiobsp.match(FIR_DB, FIR_CAPTURED)
	void Match(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();

		if (args.Length() == 2) {
			NBioAPI_BOOL result;
			NBioAPI_INPUT_FIR inputFirCaptured, inputFirStored;

			v8::String::Utf8Value storedFromArgs(isolate, args[0]);
			std::string firStored = std::string(*storedFromArgs);

			v8::String::Utf8Value capturedFromArgs(isolate, args[1]);
			std::string firCaptured = std::string(*capturedFromArgs);

			// Convertendo para string as FIRs do argumento do método.
			std::string valueFirCaptured(firCaptured);
			std::string valueFirStored(firStored);

			// Extraindo os tamanhos dos valores do template capturado e armazenado.
			int lenFirCaptured = strlen(valueFirCaptured.c_str());
			int lenFirStored = strlen(valueFirStored.c_str());

			// Extraindo o hash do template FIR capturado.
			NBioAPI_FIR_TEXTENCODE textFirCaptured;
			textFirCaptured.IsWideChar = NBioAPI_FALSE;
			textFirCaptured.TextFIR = new NBioAPI_CHAR[lenFirCaptured + 1];
			memcpy(textFirCaptured.TextFIR, valueFirCaptured.c_str(), lenFirCaptured + 1);

			// Registrando o hash do template FIR no Input de FIR capturado.
			inputFirCaptured.Form = NBioAPI_FIR_FORM_TEXTENCODE;
			inputFirCaptured.InputFIR.TextFIR = &textFirCaptured;

			// Extraindo o hash do template FIR armazenado.
			NBioAPI_FIR_TEXTENCODE textFirStored;
			textFirStored.IsWideChar = NBioAPI_FALSE;
			textFirStored.TextFIR = new NBioAPI_CHAR[lenFirStored + 1];
			memcpy(textFirStored.TextFIR, valueFirStored.c_str(), lenFirStored + 1);

			// Registrando o hash do template FIR no Input de FIR armazenado.
			inputFirStored.Form = NBioAPI_FIR_FORM_TEXTENCODE;
			inputFirStored.InputFIR.TextFIR = &textFirStored;

			// Fazer match das FIR e armazenar o resultado em result.
			ret = NBioAPI_VerifyMatch(
				bspHandle,      	// Manipulador do módulo NBioBSP
				&inputFirCaptured, 	// Primeira FIR
				&inputFirStored,  	// Segunda FIR
				&result,    		// Resultado da comparação
				NULL    		// Payload
			);

			if (result == NBioAPI_TRUE) {
				args.GetReturnValue().Set(Boolean::New(isolate, true));
			}
			else {
				args.GetReturnValue().Set(Number::New(isolate, ret));
			}
		}
		else {
			args.GetReturnValue().Set(Number::New(isolate, ERROR_MISSING_ARGS));
		}
	}

	void Close(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		ret = NBioAPI_CloseDevice(bspHandle, idDevice);
		if (ret == NBioAPIERROR_NONE) {
			ret = NBioAPI_Terminate(bspHandle);
			if (ret == NBioAPIERROR_NONE) {
				args.GetReturnValue().Set(Boolean::New(isolate, true));
				return;
			}
		}

		args.GetReturnValue().Set(Number::New(isolate, ret)); 
	}

	void init(Local<Object> exports) {
		NODE_SET_METHOD(exports, "init", InitModule);
		NODE_SET_METHOD(exports, "capture", Capture);
		NODE_SET_METHOD(exports, "match", Match);
		NODE_SET_METHOD(exports, "setSkin", SetSkin);
		NODE_SET_METHOD(exports, "close", Close);

		// Export original purpose constants
		NODE_DEFINE_CONSTANT(exports, NBioAPI_FIR_PURPOSE_AUDIT);
		NODE_DEFINE_CONSTANT(exports, NBioAPI_FIR_PURPOSE_ENROLL);
		NODE_DEFINE_CONSTANT(exports, NBioAPI_FIR_PURPOSE_VERIFY);
		NODE_DEFINE_CONSTANT(exports, NBioAPI_FIR_PURPOSE_IDENTIFY);
		
		// Export shorten purpose constants
		NODE_DEFINE_CONSTANT(exports, AUDIT);
		NODE_DEFINE_CONSTANT(exports, ENROLL);
		NODE_DEFINE_CONSTANT(exports, VERIFY);
		NODE_DEFINE_CONSTANT(exports, IDENTIFY);

		// Export all error codes from NBioAPI_Error.h
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_NONE);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INVALID_HANDLE);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INVALID_POINTER);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INVALID_TYPE);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_FUNCTION_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_STRUCTTYPE_NOT_MATCHED);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_ALREADY_PROCESSED);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_EXTRACTION_OPEN_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_VERIFICATION_OPEN_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_DATA_PROCESS_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_MUST_BE_PROCESSED_DATA);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INTERNAL_CHECKSUM_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_ENCRYPTED_DATA_ERROR);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_UNKNOWN_FORMAT);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_UNKNOWN_VERSION);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_VALIDITY_FAIL);

		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_MAXFINGER);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_SAMPLESPERFINGER);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_ENROLLQUALITY);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_VERIFYQUALITY);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_IDENTIFYQUALITY);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_SECURITYLEVEL);

		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INVALID_MINSIZE);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INVALID_TEMPLATE);

		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_EXPIRED_VERSION);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INVALID_SAMPLESPERFINGER);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_UNKNOWN_INPUTFORMAT);

		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_ENROLLSECURITYLEVEL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_NECESSARYENROLLNUM);
		// NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_RESERVED1);
		// NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_RESERVED2);
		// NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_RESERVED3);
		// NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_RESERVED4);
		// NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_RESERVED5);
		// NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_RESERVED6);
		// NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_RESERVED7);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_OUT_OF_MEMORY);

		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_DEVICE_OPEN_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INVALID_DEVICE_ID);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_WRONG_DEVICE_ID);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_DEVICE_ALREADY_OPENED);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_DEVICE_NOT_OPENED);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_DEVICE_BRIGHTNESS);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_DEVICE_CONTRAST);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_DEVICE_GAIN);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_LOWVERSION_DRIVER);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_DEVICE_INIT_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_DEVICE_LOST_DEVICE);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_DEVICE_DLL_LOAD_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_DEVICE_MAKE_INSTANCE_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_DEVICE_DLL_GET_PROC_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_DEVICE_IO_CONTROL_FAIL);

		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_USER_CANCEL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_USER_BACK);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_CAPTURE_TIMEOUT);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_CAPTURE_FAKE_SUSPICIOUS);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_ENROLL_EVENT_PLACE);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_ENROLL_EVENT_HOLD);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_ENROLL_EVENT_REMOVE);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_ENROLL_EVENT_PLACE_AGAIN);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_ENROLL_EVENT_EXTRACT);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_ENROLL_EVENT_MATCH_FAILED);

		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_MAXCANDIDATE);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_NSEARCH_OPEN_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_NSEARCH_INIT_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_NSEARCH_MEM_OVERFLOW);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_NSEARCH_SAVE_DB);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_NSEARCH_LOAD_DB);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_NSEARCH_INVALD_TEMPLATE);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_NSEARCH_OVER_LIMIT);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_NSEARCH_IDENTIFY_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_NSEARCH_LICENSE_LOAD);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_NSEARCH_LICENSE_KEY);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_NSEARCH_LICENSE_EXPIRED);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_NSEARCH_DUPLICATED_ID);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_NSEARCH_INVALID_ID);

		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_IMGCONV_INVALID_PARAM);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_IMGCONV_MEMALLOC_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_IMGCONV_FILEOPEN_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_IMGCONV_FILEWRITE_FAIL);

		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INIT_PRESEARCHRATE);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INDEXSEARCH_INIT_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INDEXSEARCH_SAVE_DB);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INDEXSEARCH_LOAD_DB);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INDEXSEARCH_UNKNOWN_VER);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INDEXSEARCH_IDENTIFY_FAIL);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INDEXSEARCH_DUPLICATED_ID);
		NODE_DEFINE_CONSTANT(exports, NBioAPIERROR_INDEXSEARCH_IDENTIFY_STOP);
		
		// Export custom error codes from nbiobsp.h
		NODE_DEFINE_CONSTANT(exports, ERROR_MISSING_ARGS);
	}

	NODE_MODULE(NBioBSP, init)

}