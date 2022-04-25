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
		BPs.Add(LoadObject<UClass>(NULL, TEXT("Blueprint'/Game/Asset/Actor/Object/BP_Target.BP_Target_C'")));
		BPs.Add(LoadObject<UClass>(NULL, TEXT("Blueprint'/Game/Asset/Actor/Object/BP_Door.BP_Door_C'")));
		BPs.Add(LoadObject<UClass>(NULL, TEXT("Blueprint'/Game/Asset/Actor/Object/BP_Box.BP_Box_C'")));
		BPs.Add(LoadObject<UClass>(NULL, TEXT("Blueprint'/Game/Asset/Actor/Object/BP_Destructible.BP_Destructible_C'")));
		BPs.Add(LoadObject<UClass>(NULL, TEXT("Blueprint'/Game/Asset/Actor/Object/BP_PlayzoneTarget.BP_PlayzoneTarget_C'")));
		BPs.Add(LoadObject<UClass>(NULL, TEXT("Blueprint'/Game/Asset/Actor/Object/BP_TargetDoll.BP_TargetDoll_C'")));
		
		// 오브젝트 아이디를 저장할 저장소
		TMap<UClass*, int> ids;

		// 월드에 스폰된 액터를 순회하면서 오브젝트를 검사
		for (const auto& i : TActorRange<AActor>(GetWorld()))
		{
			// 블루프린트 에셋과 검색한 액터가 같다면 아이디를 부여하고 저장
			if (BPs.Contains(i->GetClass()))
			{
				// 블루프린트 함수 호출 테스트 코드
				if (i->GetClass() == BPs[0])
				{
					FOutputDeviceNull ar;
					i->CallFunctionByNameWithArguments(TEXT("Test"), ar, NULL, true);
				}
				int& key = ids.FindOrAdd(i->GetClass());
				key++;

				int index;
				BPs.Find(i->GetClass(), index);

				actorList.Add(((index + 3) * 10000) + key, i);
			}
		}
		if (out.is_open())
		{
			// 캐릭터 정보 (나중에 정리 예정)
			out << "[10001][1][0][-680/189.999/92][0/0/-90][1/1/1]" << std::endl;
			out << "[20001][1][0][1712/216.999/92][0/0/-90][1/1/1]" << std::endl;

			for (const auto& i : actorList)
			{
				UE_LOG(LogTemp, Warning, TEXT("%d, %s"), i.Key, *i.Value->GetName());

				FProperty* Property = i.Value->GetClass()->FindPropertyByName(TEXT("Mesh ID"));
				if (Property)
				{
					int *meshId = Property->ContainerPtrToValuePtr<int>(i.Value);

					if (meshId)
					{	// id / level id / mesh id / transform(loc, rot, scale)
						out << "[" << i.Key << "]" << "[" << 1 << "]"
							<< "["
							<< *meshId
							<< "]"
							<< "["
							<< i.Value->GetActorLocation().X << "/"
							<< i.Value->GetActorLocation().Y << "/"
							<< i.Value->GetActorLocation().Z
							<< "]"
							<< "["
							<< i.Value->GetActorRotation().Pitch << "/"
							<< i.Value->GetActorRotation().Yaw << "/"
							<< i.Value->GetActorRotation().Roll
							<< "]"
							<< "["
							<< i.Value->GetActorScale().X << "/"
							<< i.Value->GetActorScale().Y << "/"
							<< i.Value->GetActorScale().Z
							<< "]"
							<< std::endl;	
					}
				}
			}
		}
	}
}
