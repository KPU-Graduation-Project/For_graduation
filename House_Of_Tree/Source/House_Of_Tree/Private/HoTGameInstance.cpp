// Fill out your copyright notice in the Description page of Project Settings.


#include "HoTGameInstance.h"
#include <fstream>
#include "VRPlayerController_Base.h"
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

		// 블루 프린트 에셋을 로드해와서 저장
		int key = 1, id = 1;
		for (const auto i : BP_Char)
		{
			bpSet.Add((id * 100000) + key, i->GeneratedClass);
			key++;
		}

		key = 1;
		id++;
		for (const auto i : BP_Monster)
		{
			bpSet.Add((id * 100000) + key, i->GeneratedClass);
			key++;
		}

		key = 1;
		id++;
		for (const auto i : BP_Bullet)
		{
			bpSet.Add((id * 100000) + key, i->GeneratedClass);
			key++;
		}

		key = 1;
		id++;
		for (const auto i : BP_DyObj)
		{
			bpSet.Add((id * 100000) + key, i->GeneratedClass);
			key++;
		}

		key = 1;
		id++;
		for (const auto i : BP_PasObj)
		{
			bpSet.Add((id * 100000) + key, i->GeneratedClass);
			key++;
		}

		if (out.is_open())
		{
			// 좌표값들 반올림해서 출력
			out << "[1][0][00100001][-680/189.999/92][0/0/-90][1/1/1]" << std::endl;
			out << "[1][0][00100002][1712/216.999/92][0/0/-90][1/1/1]" << std::endl;
			// FActorSpawnParameters SpawnParams;
			// GetWorld()->SpawnActor<AActor>(, FVector(0, 0, 0), FRotator(0, 0,0), SpawnParams);

			// 월드에 스폰된 액터를 순회하면서 오브젝트를 검사
			for (const auto& i : TActorRange<AActor>(GetWorld()))
			{
				// 블루프린트 에셋과 검색한 액터가 같다면 데이터 파일에 기록
				//if (bpSet.Contains(i->GetClass()))
				const int *objectID = bpSet.FindKey(i->GetClass());
				if (objectID != nullptr)
				{
					FProperty* Property = i->GetClass()->FindPropertyByName(TEXT("Mesh ID"));
					if (Property)
					{
						int *meshId = Property->ContainerPtrToValuePtr<int>(i);

						UE_LOG(LogTemp, Warning, TEXT("%s"), *i->GetName());

						// Level
						out << "[" << 1 << "]";

						// Object ID
						out << "[";
						out.width(8);
						out.fill('0');
						//out << bpSet[i->GetClass()];
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
						// FProperty* variables = i->GetClass()->FindPropertyByName(TEXT("Actor"));
						// if (variables != nullptr)
						// {
						// 	UClass* target = variables->ContainerPtrToValuePtr<UClass>(i);
						// 	const int *targetID = bpSet.FindKey(i->GetClass());
						// 	
						// 	out << "[";
						// 	out.width(8);
						// 	out.fill('0');
						// 	//out << bpSet[i->GetClass()];
						// 	out << *objectID;
						// 	out << "]";
						// }

						out << std::endl;
					}
				}
			}
		}
		out.close();
	}
}

void UHoTGameInstance::SetPlayer(int playerId)
{
	playerController->SetPlayerCharacter(playerId, actorList[playerId]);
}

void UHoTGameInstance::PutObject(int actorID, int objectID, FVector location, FRotator rotation, FVector scale)
{
	FTransform transform;
	transform.SetLocation(location);
	transform.SetRotation(rotation.Quaternion());
	transform.SetScale3D(scale);
	FActorSpawnParameters SpawnParams;

	actorList.Add(actorID, GetWorld()->SpawnActor(bpSet[objectID], &transform, SpawnParams));

	if (objectID == 00100001 || objectID == 00100002)
	{
		SetPlayer(actorID);
	}
}