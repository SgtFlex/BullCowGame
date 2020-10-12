// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
//#incldue "Math/UnrealMathUtility.h" //Already included in CoreMinimal.h from the header file

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");

    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString& Word)
    {
         return Word.Len() > 4 && Word.Len() < 8 && IsIsogram(Word);
    });

    SetupGame(); //Setting up Game    
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else //check playerguess
    {
        ProcessGuess(Input);
    }    
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0,Isograms.Num() - 1)];
    PlayerLives = HiddenWord.Len() * 2;
    GuessHistory.Empty();
    bGameOver = false;
    PrintLine(TEXT("Welcome! Press 'Tab' to interact."), HiddenWord.Len());
    PrintLine(TEXT("Guess the %i letter isogram."), HiddenWord.Len());
    PrintLine(TEXT("Bulls = Correct letter in the correct spot"));
    PrintLine(TEXT("Cows = Correct letter in the wrong spot."));
    PrintLine(TEXT("You have %i Lives."), PlayerLives);
    PrintLine(TEXT("Please input a guess and press 'Enter.'"));
    PrintLine(TEXT("Type 'Words' to see what you have entered."));
    // PrintLine(TEXT("The HiddenWord is: %s"), *HiddenWord);
    IsIsogram(HiddenWord);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    
    PrintLine(TEXT("Press 'Enter' to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    //Check if isogram
    //Prompt to GuessAgain
    //Check if correct # of letters
    //Prompt to GuessAgain

    //Remove Life
    
    //Show Lives left
    //Check if Lives > 0
    //If Yes, GuessAgain
    //Else, Prompt to PlayAgain
    //If Yes, Restart
    //If No, Exit
    if (Guess=="Words")
    {
        PrintHistory();
        return;
    }
    else if (Guess==HiddenWord)
    {
        PrintLine(TEXT("You got it right!"));
        EndGame();
        return;
    }
    else if (Guess.Len()!=HiddenWord.Len())
    {
        PrintLine(TEXT("Wrong length! The isogram is %i letters"), HiddenWord.Len());
        return;
    }
    else if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters"));
        return;
    }
    PrintLine(TEXT("Wrong, %i lives left"), --PlayerLives);
    if (PlayerLives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("No Lives left. Game Over."));
        PrintLine(TEXT("The hidden isogram was: %s"), *HiddenWord);
        EndGame();
        return;
    }
    FBullCowCount Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    GuessHistory.Emplace(Score);
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    //Nested for loop checking each letter forwards
    for (int32 Index = 0; Index < Word.Len(); Index++)  
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Comparison] == Word[Index])
            {
                return false;
            }
        }
        
    }
    return true;   
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    Count.CheckWord = Guess;
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break; //Because it's an isogram, there's no reason to continue once we find a matching letter
            }
        }
    }
    return Count;
}

void UBullCowCartridge::PrintHistory() const
{
    for (int32 Index = 0; Index < GuessHistory.Num(); Index++)
    {
        PrintLine(TEXT("Word: %s, %i Bulls, %i Cows"), *GuessHistory[Index].CheckWord, GuessHistory[Index].Bulls, GuessHistory[Index].Cows);
    }
}