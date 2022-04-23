// Fill out your copyright notice in the Description page of Project Settings.


#include "HoTGameInstance.h"

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
}

void UHoTGameInstance::OnStart()
{
	Super::OnStart();

	if (ConnectNetwork)
	{
		InitSocket();
	}

	if (makeIDList)
	{
		// 기록할 스태틱 오브젝트들을 선언 (나중에 파싱하는 방식으로 변경)
		TArray<FString> objests = {
			TEXT("SM_box"), TEXT("SM_door_02"), TEXT("SM_door_03"), TEXT("SM_doorbranch_01"), TEXT("SM_branch_01"),
			TEXT("SM_target_01"), TEXT("SM_targetdoll_01"), TEXT("SM_branch_02"), TEXT("SM_Target_Top")
		};

		// 블루 프린트 에셋을 로드해와서 저장 (나중에 블루프린트에서 선택한 것을 가져오는 것으로 변경 할 예정)
		TArray<UBlueprintGeneratedClass*> LoadedBP = {
			LoadObject<UBlueprintGeneratedClass>(
				NULL, TEXT("Blueprint'/Game/Asset/Actor/Object/BP_Target.BP_Target_C'")),
			LoadObject<UBlueprintGeneratedClass>(NULL, TEXT("Blueprint'/Game/Asset/Actor/Object/BP_Door.BP_Door_C'"))
		};

		// 오브젝트 아이디를 저장할 저장소
		TMap<FString, int> ids;

		UStaticMeshComponent* tempComponent = nullptr;
		// 월드에 스폰된 액터를 순회하면서 오브젝트를 검사
		for (const auto& i : TActorRange<AActor>(GetWorld()))
		{
			// 블루프린트 에셋과 검색한 액터가 같다면 아이디를 부여하고 저장
			if (LoadedBP.Contains(i->GetClass()))
			{
				//UE_LOG(LogTemp, Error, TEXT("%s"), *i->GetName());
				tempComponent = Cast<UStaticMeshComponent>(i->GetComponentByClass(UStaticMeshComponent::StaticClass()));
				if (tempComponent && tempComponent->GetStaticMesh())
				{
					int& key = ids.FindOrAdd(*tempComponent->GetStaticMesh()->GetName());
					key++;

					//UE_LOG(LogTemp, Warning, TEXT("%s %d"), *(Cast<UStaticMeshComponent>(tmp))->GetStaticMesh()->GetName(), key);

					int t = objests.Find(*tempComponent->GetStaticMesh()->GetName());

					objList.Add(((t + 1) * 10000) + key, i);
				}
			}

			// else if (i->GetClass() == LoadedBP1)
			// {
			// 	//UE_LOG(LogTemp, Error, TEXT("%s"), *i->GetName());
			// 	tempComponent = Cast<UStaticMeshComponent>(
			// 		i->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			// 	if (tempComponent)
			// 	{
			// 		int& key = ids.FindOrAdd(*tempComponent->GetStaticMesh()->GetName());
			// 		key++;
			//
			// 		//UE_LOG(LogTemp, Warning, TEXT("%s %d"), *(Cast<UStaticMeshComponent>(tmp))->GetStaticMesh()->GetName(), key);
			//
			// 		int t = objests.Find(*tempComponent->GetStaticMesh()->GetName());
			//
			// 		objList.Add(((t + 1) * 10000) + key, i);
			// 	}
			// }
		}

		for (const auto& i : objList)
		{
			UE_LOG(LogTemp, Warning, TEXT("%d, %s"), i.Key, *i.Value->GetName());
		}
	}
}

void UHoTGameInstance::InitSocket()
{
	if (SocketInstance != nullptr) return;

	SocketInstance = new ClientSocket(this);
}
