// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerMenuPC.h"

void AServerMenuPC::ServerTravel(const FString& mapPath) {
    GetWorld()->ServerTravel(mapPath);
}