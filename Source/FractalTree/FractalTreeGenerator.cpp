
#include "FractalTreeGenerator.h"

#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

//sets default values
AFractalTreeGenerator::AFractalTreeGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

//called when the game starts or when spawned
void AFractalTreeGenerator::BeginPlay()
{
    Super::BeginPlay();

    //convert branch height and radius factors from percent to decimal
    BranchHeightFactor = BranchHeightMultiplier * 0.01f;
    BranchRadiusFactor = BranchRadiusMultiplier * 0.01f;

    double startTime = FPlatformTime::Seconds();

    //generate the tree
    GenerateFractalTree(NumTreeLevels, StartPosition, FRotator::ZeroRotator, TrunkHeight);

    double endTime = FPlatformTime::Seconds();
    //display generation time
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Generation Time: %f Milliseconds"), (endTime - startTime) * 1000.0f));

    //ensure any new components are registered
    this->RegisterAllComponents();
}

//called every frame
void AFractalTreeGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AFractalTreeGenerator::GenerateFractalTree(int Iterations, FVector StartLocation, FRotator Rotation, float Length)
{
    if (Iterations <= 0) return; //final fractal depth

    if (Iterations != NumTreeLevels && FMath::RandRange(0, 100) > BranchChance) return; //branch skipped from branchChance probability

    if (Iterations == NumTreeLevels - 1) Length = BaseBranchHeight; //first branch visually (after tree trunk)

    //calculate end location of this branch
    FVector EndLocation = StartLocation + Rotation.Vector() * Length;
    EndLocation = FQuat(FRotator(90.f, 0.f, 0.f)) * (EndLocation - StartLocation) + StartLocation;

    //if enabled, draw debug line for this branch
    if (DrawTreeDebugLine) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1, 0, Iterations * 2);

    //if enabled, create cylinder for this branch
    if (DrawTreeMesh) GenerateCylinder(Iterations, StartLocation, EndLocation, Rotation, Length);

    if (NumOfBranches == 2)
    {
        //generate branches left and right
        GenerateFractalTree(Iterations - 1, EndLocation, Rotation + GetAdditionalBranchRotation(FRotator(0.0f, 30.0f, 0.0f)), Length * BranchHeightFactor);
        GenerateFractalTree(Iterations - 1, EndLocation, Rotation + GetAdditionalBranchRotation(FRotator(0.0f, -30.0f, 0.0f)), Length * BranchHeightFactor);
    }
    else if (NumOfBranches == 3)
    {
        //generate branches forwards, backwards and left, and backwards and right
        GenerateFractalTree(Iterations - 1, EndLocation, Rotation + GetAdditionalBranchRotation(FRotator(15.0f, 30.0f, 0.0f)), Length * BranchHeightFactor);
        GenerateFractalTree(Iterations - 1, EndLocation, Rotation + GetAdditionalBranchRotation(FRotator(15.0f, -30.0f, 0.0f)), Length * BranchHeightFactor);
        GenerateFractalTree(Iterations - 1, EndLocation, Rotation + GetAdditionalBranchRotation(FRotator(-30.0f, 0.0f, 0.0f)), Length * BranchHeightFactor);
    }
    else if (NumOfBranches == 4)
    {
        //generate branches forwards, left, right and backwards
        GenerateFractalTree(Iterations - 1, EndLocation, Rotation + GetAdditionalBranchRotation(FRotator(0.0f, 30.0f, 0.0f)), Length * BranchHeightFactor);
        GenerateFractalTree(Iterations - 1, EndLocation, Rotation + GetAdditionalBranchRotation(FRotator(0.0f, -30.0f, 0.0f)), Length * BranchHeightFactor);
        GenerateFractalTree(Iterations - 1, EndLocation, Rotation + GetAdditionalBranchRotation(FRotator(30.0f, 0.0f, 0.0f)), Length * BranchHeightFactor);
        GenerateFractalTree(Iterations - 1, EndLocation, Rotation + GetAdditionalBranchRotation(FRotator(-30.0f, 0.0f, 0.0f)), Length * BranchHeightFactor);
    }
}

FRotator AFractalTreeGenerator::GetAdditionalBranchRotation(FRotator BaseRotation)
{
    if (!AddBranchRandomRotation) return BaseRotation; //if not enabled, do not add random rotation

    //apply randomn value to rotation on each axis
    BaseRotation.Roll += FMath::RandRange(-BranchRotationRandomRange, BranchRotationRandomRange);
    BaseRotation.Pitch += FMath::RandRange(-BranchRotationRandomRange, BranchRotationRandomRange);
    BaseRotation.Yaw += FMath::RandRange(-BranchRotationRandomRange, BranchRotationRandomRange);

    return BaseRotation;
}

void AFractalTreeGenerator::GenerateCylinder(int Iteration, FVector Start, FVector End, FRotator Rotation, float Length)
{
    //calculate radius based on branch depth
    float Radius;
    if (Iteration == NumTreeLevels) Radius = TrunkRadius;
    else if (Iteration == NumTreeLevels - 1) Radius = BaseBranchRadius;
    else Radius = BaseBranchRadius * FMath::Pow(BranchRadiusFactor, NumTreeLevels - 1 - Iteration);

    //convert radius and length to world scale
    Radius *= 0.1f;
    Length *= 0.01f;

    //multiply variables to tree scale
    Start *= TreeScale;
    End *= TreeScale;
    Radius *= TreeScale;
    Length *= TreeScale;

    //create static mesh component for cylinder
    UStaticMeshComponent* Cylinder = NewObject<UStaticMeshComponent>(this);
    Cylinder->SetStaticMesh(BranchCylinderMesh);
    Cylinder->SetMaterial(0, CylinderMaterial);
    Cylinder->SetWorldLocation(Start);

    //convert branch rotation to world rotation value
    if (Rotation.IsNearlyZero(0.1f)) Cylinder->SetWorldRotation(Rotation);
    else
    {
        FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
        NewRotation.Pitch -= 90.0f; //rotate to default facing upwards in scene
        Cylinder->SetWorldRotation(NewRotation);
    }

    //set world scale and attach to root component
    Cylinder->SetWorldScale3D(FVector(Radius, Radius, Length));
    Cylinder->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}
