
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "FractalTreeGenerator.generated.h"

UCLASS()
class FRACTALTREE_API AFractalTreeGenerator : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AFractalTreeGenerator();

    UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1", ClampMax = "10", Delta = 0.1f, Multiple = 0.1f), Category = "Tree Settings")
        float TreeScale = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Tree Settings")
        FVector StartPosition = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, meta = (ClampMin = "2", ClampMax = "7"), Category = "Tree Settings")
        int NumTreeLevels = 5;

    UPROPERTY(EditAnywhere, meta = (ClampMin = "2", ClampMax = "4"), Category = "Tree Settings")
        int NumOfBranches = 4;

    UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "100"), DisplayName = "Branch Chance (%)", Category = "Tree Settings")
        int BranchChance = 100;

    UPROPERTY(EditAnywhere, Category = "Tree Settings")
        bool AddBranchRandomRotation = false;

    UPROPERTY(EditAnywhere, meta = (ClampMin = "5", ClampMax = "45"), Category = "Tree Settings")
        int BranchRotationRandomRange = 10;

    UPROPERTY(EditAnywhere, Category = "Tree Settings")
        bool DrawTreeMesh = false;

    UPROPERTY(EditAnywhere, Category = "Tree Settings")
        bool DrawTreeDebugLine = true;

    UPROPERTY(EditAnywhere, Category = "Tree Settings")
        UMaterialInterface* CylinderMaterial;

    UPROPERTY(EditAnywhere, Category = "Tree Settings")
        UStaticMesh* BranchCylinderMesh;

    UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "500"), Category = "Tree Settings|Tree Dimensions")
        int TrunkHeight = 200;

    UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1", ClampMax = "20", Delta = 0.1f, Multiple = 0.1f), Category = "Tree Settings|Tree Dimensions")
        float TrunkRadius = 2.0f;

    UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "500"), Category = "Tree Settings|Tree Dimensions")
        int BaseBranchHeight = 150;

    UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "100"), DisplayName = "Branch Height Multiplier (%)", Category = "Tree Settings|Tree Dimensions")
        int BranchHeightMultiplier = 75;

    UPROPERTY(EditAnywhere, meta = (ClampMin = "0.1", ClampMax = "20", Delta = 0.1f, Multiple = 0.1f), Category = "Tree Settings|Tree Dimensions")
        float BaseBranchRadius = 1.5f;

    UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "100"), DisplayName = "Branch Radius Multiplier (%)", Category = "Tree Settings|Tree Dimensions")
        int BranchRadiusMultiplier = 75;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:

    void GenerateFractalTree(int Iterations, FVector StartLocation, FRotator Rotation, float Length);
    void GenerateCylinder(int Iteration, FVector Start, FVector End, FRotator Rotation, float Length);

    FRotator GetAdditionalBranchRotation(FRotator BaseRotation);

    float BranchHeightFactor;
    float BranchRadiusFactor;

};
