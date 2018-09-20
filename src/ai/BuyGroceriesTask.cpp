#include "BuyGroceriesTask.h"

#include "Character.h"
#include "Faction.h"
#include "WalkTask.h"

AITask *BuyGroceriesTask::act(Character &C, float DTime) {
  Seller = C.getFaction()->closestFoodOffer(C.getTilePos());
  if (!Seller)
    return finish("Couldn't find a seller");

  if (C.getTilePos().distance(Seller->getTilePos()) >= 2)
    return new WalkTask(Seller->getTilePos());

  for (int i = 0; i < 10; ++i)
    C.getPrivateInventory().buyItem(Seller->getPrivateInventory(), "corn");
  return finish("Bought groceries");
}
