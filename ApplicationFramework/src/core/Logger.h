#pragma once
#include "Core.h"
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Engine {
	struct LogMessage
	{
		std::string msg;
		glm::vec3 color;
		int type;
	};

	class Logger {
	public:
		
		inline static void LogError(std::string msg);
		inline static void LogWarning(std::string msg);
		inline static void LogInfo(std::string msg);

		inline static std::vector<LogMessage>& GetLog() { return m_Log; }

	private:
		inline static std::vector<LogMessage> m_Log;
	};

	void Logger::LogError(std::string msg)
	{
		//m_Log.push_back({ msg, {1,0,1}, 0 });
		std::cout << "Error {\n" << msg << "\n}\n";
	}
	void Logger::LogWarning(std::string msg)
	{
		//m_Log.push_back({ msg, {1,1,0}, 1 });
		std::cout << "Warning {\n" << msg << "\n}\n";
	}
	void Logger::LogInfo(std::string msg)
	{
		//m_Log.push_back({ msg, {0,1,1}, 2 });
		std::cout << "Info {\n" << msg << "\n}\n";
	}
}