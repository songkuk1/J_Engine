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
			// �߰����� �ʱ�ȭ �۾��� ���⿡ �ۼ��մϴ�.
			// ��: DirectX ���ҽ� ����, ���̴� �ε� ��
			return true;
		}


  
    };
