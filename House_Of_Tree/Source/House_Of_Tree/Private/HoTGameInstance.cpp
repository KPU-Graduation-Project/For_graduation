// Fill out your copyright notice in the Description page of Project Settings.
#include "HoTGameInstance.h"
#include "VRPlayerController_Base.h"
#include "EngineUtils.h"

#include <fstream>

#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"
#include "AI/NavDataGenerator.h"
#include "NavMesh/RecastNavMesh.h"


UHoTGameInstance::UHoTGameInstance()
{
	SocketInstance = nullptr;
}

UHoTGameInstance::~UHoTGameInstance()
{
}

void UHoTGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogInit, Warning, TEXT("Instance Initialized"));

	path = FPaths::ProjectUserDir();
	path.Append(TEXT("data.txt"));

	ipPath = FPaths::ProjectUserDir();
	ipPath.Append(TEXT("IP_ADDRESS.txt"));
}

void UHoTGameInstance::InitSocket()
{
	if (!ConnectNetwork) return;
	if (SocketInstance != nullptr) return;

	std::ifstream in(*ipPath);
	if (in.is_open())
	{
		char a[100];
		in >> a;
		UE_LOG(LogInit, Warning, TEXT("%s"), ToCStr(a));
		ipAddr = ToCStr(a);
	}
	else
	{
		ipAddr = TEXT("127.0.0.1");
	}
	in.close();
	
	SocketInstance = new ClientSocket(this);
}

void UHoTGameInstance::SetInfo()
{
	// 네비 메쉬
	if (ExportNavMesh)
	{
		FWorldContext Context = GEngine->GetWorldContexts().Last();
		UWorld *World = Context.World();
		const UNavigationSystemV1 *NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
		const ANavigationData *NavData = NavSys->GetDefaultNavDataInstance();
		const ARecastNavMesh *NavMesh = Cast<ARecastNavMesh>(NavData);
		NavMesh->GetGenerator()->ExportNavigationData(FString(TEXT("C:\\Users\\Kclient\\Testnavmesh.obj")));
		UE_LOG(LogInit, Warning, TEXT("NavMeshExported"));
	}

	if (bpSet.Num() == 0)
	{
		// 블루 프린트 에셋을 로드해와서 저장
		int key = 1, id = 1;
		for (const auto i : BP_Char)
		{
			bpSet.Add((id * 100000) + key, i.Get());
			key++;
		}

		key = 1;
		id++;
		for (const auto i : BP_Monster)
		{
			bpSet.Add((id * 100000) + key, i.Get());
			key++;
		}

		key = 1;
		id++;
		for (const auto i : BP_Bullet)
		{
			bpSet.Add((id * 100000) + key, i.Get());
			key++;
		}

		key = 1;
		id++;
		for (const auto i : BP_DyObj)
		{
			bpSet.Add((id * 100000) + key, i.Get());
			key++;
		}

		key = 1;
		id++;
		for (const auto i : BP_PasObj)
		{
			bpSet.Add((id * 100000) + key, i.Get());
			key++;
		}
	}

	if (makeIDList)
	{
		TArray<AActor*> actors;
		
		// 월드에 스폰된 액터를 순회하면서 오브젝트를 검사후 배열에 넣어둠
		for (const auto& i : TActorRange<AActor>(GetWorld()))
		{
			// 블루프린트 에셋과 검색한 액터가 같다면 데이터 파일에 기록
			if (bpSet.FindKey(i->GetClass()) == nullptr) continue;

			actors.Push(i);
		}
		
		std::ofstream out(*path);

		if (out.is_open())
		{
			int num = 0;
			
			// 캐릭터
			out << "[" << num++ << "]" << "[1][00100001][0][-68000/19000/9200][0/0/-9000][100/100/100]" << std::endl;
			out << "[" << num++ << "]" << "[1][00100002][0][171200/21700/9200][0/0/-9000][100/100/100]" << std::endl;

			for (const auto& i : actors)
			{
				FProperty* Property = i->GetClass()->FindPropertyByName(TEXT("Mesh ID"));
				if (!Property) continue;

				int *meshId = Property->ContainerPtrToValuePtr<int>(i);
				const int *objectID = bpSet.FindKey(i->GetClass());

				// Index
				out << "[" << num++ << "]";
				
				// Level
				out << "[" << 1 << "]";
				
				// Object ID
				out << "[";
				out.width(8);
				out.fill('0');
				out << *objectID;
				out << "]";
				
				// Mesh ID
				out << "[" << *meshId << "]";
				
				// Location
				out << "["
					<< static_cast<int>(i->GetActorLocation().X * 100) << "/"
					<< static_cast<int>(i->GetActorLocation().Y * 100) << "/"
					<< static_cast<int>(i->GetActorLocation().Z * 100)
					<< "]";
				
				// Rotation
				out << "["
					<< static_cast<int>(i->GetActorRotation().Pitch * 100) << "/"
					<< static_cast<int>(i->GetActorRotation().Yaw * 100) << "/"
					<< static_cast<int>(i->GetActorRotation().Roll * 100)
					<< "]";
				
				// Scale
				out << "["
					<< static_cast<int>(i->GetActorScale().X * 100) << "/"
					<< static_cast<int>(i->GetActorScale().Y * 100) << "/"
					<< static_cast<int>(i->GetActorScale().Z * 100)
					<< "]";
				
				// Any Other Variables
				FProperty* variables = i->GetClass()->FindPropertyByName(TEXT("Actor"));
				if (variables != nullptr)
				{
					AActor** target = variables->ContainerPtrToValuePtr<AActor*>(i);
					int targetIndex = actors.Find(*target);
					
					out << "[";
					out << targetIndex + 2;	// 캐릭터 두개 만큼 추가
					out << "]";
				}
				
				// If it is Enemy
				if (BP_Monster.Find(i->GetClass()) != INDEX_NONE)
				{
					out << " // " << TCHAR_TO_ANSI(*i->GetName());
				}
				out << std::endl;
			}
		}
		out.close();
	}
}