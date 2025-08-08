#pragma once

#include <algorithm>
#include <directxtk/SimpleMath.h>
#include <iostream>
#include <memory>
#include <vector>

#include "TestApp.h"

namespace JEngine {

TestApp::TestApp() : AppBase(){}

		bool TestApp::Initialize() 
		{
			if (!AppBase::Initialize())
			{
				std::cout << "Failed to initialize AppBase." << std::endl;
				return false;
			}
			// 추가적인 초기화 작업을 여기에 작성합니다.
			// 예: DirectX 리소스 생성, 셰이더 로드 등
			return true;
		}


  
    };
