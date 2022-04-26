// Fill out your copyright notice in the Description page of Project Settings.


#include "HoTGameInstance.h"
#include <fstream>
#include "Misc/OutputDeviceNull.h"
#include "EngineUtils.h"
#include "Engine/BlueprintGeneratedClass.h"

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

	UE_LOG(LogTemp, Warning, TEXT("Instance Initialized"));

	path = FPaths::ProjectUserDir();
	path.Append(TEXT("data.txt"));
}

void UHoTGameInstance::OnStart()
{
	Super::OnStart();

	if (ConnectNetwork)
	{
		InitSocket();
	}
}

void UHoTGameInstance::InitSocket()
{
	if (SocketInstance != nullptr) return;

	SocketInstance = new ClientSocket(this);
}

void UHoTGameInstance::SetInfo()
{
	if (makeIDList)
	{
		std::ofstream out(*path);

		// 블루 프린트 에셋을 로드해와서 저장 (나중에 블루프린트에서 선택한 것을 가져오는 것으로 변경 할 예정)
		TMap<UClass*, int> bpSet;

		int key = 1, id = 1;
		for (const auto i : BP_Char)
		{
			bpSet.Add(i->GeneratedClass, (id * 10000) + key);
			key++;
		}

		key = 1;
		id++;
		for (const auto i : BP_Monster)
		{
			bpSet.Add(i->GetBlueprintClass(), (id * 10000) + key);
			key++;
		}

		key = 1;
		id++;
		for (const auto i : BP_Bullet)
		{
			bpSet.Add(i->GeneratedClass, (id * 10000) + key);
			key++;
		}

		key = 1;
		id++;
		for (const auto i : BP_DyObj)
		{
			bpSet.Add(i->GeneratedClass, (id * 10000) + key);
			key++;
		}

		key = 1;
		id++;
		for (const auto i : BP_PasObj)
		{
			bpSet.Add(i->GeneratedClass, (id * 10000) + key);
			key++;
		}

		if (out.is_open())
		{
			// 좌표값들 반올림해서 출력
			// out << "[1][00100001][-680/189.999/92][0/0/-90][1/1/1]" << std::endl;
			// out << "[1][00100002][1712/216.999/92][0/0/-90][1/1/1]" << std::endl;

			// 월드에 스폰된 액터를 순회하면서 오브젝트를 검사
			for (const auto& i : TActorRange<AActor>(GetWorld()))
			{
				// 블루프린트 에셋과 검색한 액터가 같다면 데이터 파일에 기록
				if (bpSet.Contains(i->GetClass()))
				{
					UE_LOG(LogTemp, Warning, TEXT("%s"), *i->GetName());
					out << "[" << 1 << "]";

					
					out	<< "[";
					out.width(8);
					out.fill('0');
					out << bpSet[i->GetClass()];
					out <<"]";
					
					out	<< "["
						<< i->GetActorLocation().X << "/"
						<< i->GetActorLocation().Y << "/"
						<< i->GetActorLocation().Z
						<< "]";
					
					out	<< "["
						<< i->GetActorRotation().Pitch << "/"
						<< i->GetActorRotation().Yaw << "/"
						<< i->GetActorRotation().Roll
						<< "]";
					
					out	<< "["
						<< i->GetActorScale().X << "/"
						<< i->GetActorScale().Y << "/"
						<< i->GetActorScale().Z
						<< "]";
					
					out	<< std::endl;
				}
			}
		}
		//
		// 	for (const auto& i : actorList)
		// 	{
		// 		UE_LOG(LogTemp, Warning, TEXT("%d, %s"), i.Key, *i.Value->GetName());
		//
		// 		FProperty* Property = i.Value->GetClass()->FindPropertyByName(TEXT("Mesh ID"));
		// 		if (Property)
		// 		{
		// 			int *meshId = Property->ContainerPtrToValuePtr<int>(i.Value);
		//
		// 			if (meshId)
		// 			{	// Level ID / Mesh ID / Transform(loc, rot, scale)
		// 				out << "[" << i.Key << "]" << "[" << 1 << "]"
		// 					<< "["
		// 					<< *meshId
		// 					<< "]"
		// 					<< "["
		// 					<< i.Value->GetActorLocation().X << "/"
		// 					<< i.Value->GetActorLocation().Y << "/"
		// 					<< i.Value->GetActorLocation().Z
		// 					<< "]"
		// 					<< "["
		// 					<< i.Value->GetActorRotation().Pitch << "/"
		// 					<< i.Value->GetActorRotation().Yaw << "/"
		// 					<< i.Value->GetActorRotation().Roll
		// 					<< "]"
		// 					<< "["
		// 					<< i.Value->GetActorScale().X << "/"
		// 					<< i.Value->GetActorScale().Y << "/"
		// 					<< i.Value->GetActorScale().Z
		// 					<< "]"
		// 					<< std::endl;	
		// 			}
		// 		}
		// 	}
		// }
	}
}
