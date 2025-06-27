#include "FusePDA.h"


TMap<FFusePair, FFuseRecipe> UFusePDA::GetRecipes() const
{
  return recipes;
}

FFuseRecipe  UFusePDA::FindRecipe(FName Item1, FName Item2) const
{
  FFusePair pair(Item1, Item2);
  if (recipes.Contains(pair))
  {
    return recipes.FindRef(pair); //need to use FindRef 
  }
  return FFuseRecipe();

}
