/*
basic guide to chams

this doesnt show everything perfectly that you will need, as I said you need a entity loop and process event function pre-made and working and 
you can just insert what I show

> it is VERY helpful if you have a sdk dump and are using it...


So to begin you are going to want to log the games material list, you do this inside of process event, for refrence pObject is UObject.
You are going to need a way to log, either to a file or to console.
you need to then put the code below in ur process-event hook and inject, you will log every single global object called and then print it
it will give you every materials name (you can improve this by adding a filter for "Material", "MaterialInterface", "MaterialInstanceDynamic", etc
Here is a example of what your process event would end up looking like (ofc adjust what u need to what u have)
*/

__forceinline void Hooks::ProcessEvent(UObject* pObject, UFunction* pFunction, void* params)
{
  for (int i = 0; i < pObject->GetGlobalObjects().Num(); i++)
  {
    print("%s\n", pObject->GetGlobalObjects().GetByIndex(i)->GetFullName().c_str());
  }
  
  return oProcessEvent(pObject, pFunction, params);
}

//Then after that you can find the materials list and find a material that you want, in most cases it will need to be a material that is named "Matierlal ..." only,
//variations dont usually work, then go to your entity loop, at the top of ur file anywhere stick
std::map<AActor*, bool> chams{};
//so you can have a map, if you dont know what it is google it, AActor should refrence your aactor class, most cases it is CG::AActor or something like that
//in your aactor loop do as such
//filter down to if is an actor, player, entity, object, what ever you want depending on the game
  for (int i = 0; i < Actors.Count; i++)
  {
    auto Actor = Actors[i];

    //here we need to get some info for chams / wireframe

    static UMaterialInstanceDynamic* material; //static since we only want to get this once, save lag...

    if (!material) material = UMaterialInstanceDynamic::FindObject<UMaterialInstanceDynamic>("INSERT MATERIAL NAME"); //put your material name, also checks if it isnt set and sets it only once to save lag

    static char* depth = (char*)((uintptr_t)material + 0x908); //Double check these offsets with your game, depends per game (its originally called bDisableDepthTest
    static char* wireframe = (char*)((uintptr_t)material + 0x910); //double check again

    *depth = !0; //chars you set true by !0, false by 0, and nop is 90
    *wireframe |= (2 << 1); //This should generally be the same, since it is a bitfield it will be set a bit diffrently if the game has more bitfields here

    //check ur stuff etc
    if (Actor->IsPlayer())
    {
      //Add any extra filters here if wanted, such as if they are dead, sleeping, disconnected, ceratain healtth etc...
      //you can also add a check for if you want chams enabled
      //what chams[Actor] does is it checks if the actor pointer is currently in the chams map and if its true/false and if it isnt sets it
      //chams/wireframe only needs to be set once.
      if (!chams[Actor])
      {
        Actor->MeshComponent->SetMaterial(0, material); //sets it to the position 0 on the character, if the character has more 'patches' of skin set it higher / lower
        //Actor->MeshComponent->SetMaterial(1, material); //example if you need to set it to cover more skin/character
        chams_map[Actor] = true; //sets it to true so the check above doesnt trigger again
      }
    }
  }
