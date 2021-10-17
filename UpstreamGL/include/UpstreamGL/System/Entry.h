#pragma once

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>

#define GLR_APP_IMPL(AppType)																		\
INT WINAPI wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR,_In_ INT)	\
{																									\
	int w_argc = 0;																					\
	LPWSTR* w_argv = CommandLineToArgvW(GetCommandLineW(), &w_argc);								\
	char** str = new char*[w_argc];																	\
	for (int i = 0; i < w_argc; ++i)																\
	{																								\
		int w_len = lstrlenW(w_argv[i]);															\
		int len = WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, NULL, 0, NULL, NULL);			\
		str[i] = new char[len + 1];																	\
		WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, str[i], len, NULL, NULL);					\
		str[i][len] = '\0';																			\
	}																								\
	AppType app(w_argc, str);																	\
	delete[] str;																					\
	return app.MainLoop();																			\
}																									\

#else
#define GLR_APP_IMPL(AppType)							\
int main(int argc, char* argv[])						\
{														\
	AppType app(argc, argv);							\
	return app.MainLoop();								\
}

#endif