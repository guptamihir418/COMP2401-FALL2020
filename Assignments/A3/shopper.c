//Mihir Gupta
// 101172281

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_BAG_CAPACITY 25
#define MAX_BAG_WEIGHT 5
#define MAX_CART_CAPACITY 100


// In assignment specification it did not clearly mention the type should be "GroceryItem" or "GroceryItemType"
// Same with bag and cart
// So I named them according to what I understood

// ALL THE STRUCTS
typedef struct {

        char *name;
        float cost;
        float weight;
		char isFrozen;

} GroceryItem;

typedef struct {

	GroceryItem **gitem; //store maximum of 25 items
	int numberOfItems;
	float totalWeight;

} Bag;

typedef struct {

	GroceryItem **gitem; //store maximum of 100 items
	int numberOfItemsInCart;
	Bag **packedGroceryBags;
	int numberOfPackedBags;

} Cart;

// Alotts memory to the cart and initalize it
void initialiseCart(Cart *cart){

	cart->gitem = (GroceryItem **) malloc(sizeof(GroceryItem *) * MAX_CART_CAPACITY);
	
	for (int i = 0 ; i < MAX_CART_CAPACITY; i++){

		cart->gitem[i]= NULL;
		cart->gitem[i]=(GroceryItem *) malloc(sizeof(GroceryItem));

	}
}

// Alotts memory to the bag and initalize it
void initialiseBag(Bag *bag){

	bag->gitem = (GroceryItem **) malloc(sizeof(GroceryItem *) * MAX_BAG_CAPACITY);

	for (int i = 0 ; i < MAX_BAG_CAPACITY; i++){
		bag->gitem[i]=(GroceryItem *) malloc(sizeof(GroceryItem));
	}
}


// adds item to bag
int addItemToBag(GroceryItem *item, Bag *bag){ 
	
 	if(item->weight > MAX_BAG_WEIGHT){
		return 1;
	}
	
	if((item->weight + bag->totalWeight) > (float)MAX_BAG_WEIGHT){
		return 0 ;
	}
	
	else if(bag->numberOfItems == MAX_BAG_CAPACITY){
		return 0;
	}
	
	else{
		
		bag->gitem[bag->numberOfItems] = item;
		bag->totalWeight += item->weight;
		bag->numberOfItems++;
		return 1;
	}
}
	

// Removes item from the bag
int removeItemFromBag(GroceryItem *item, Bag *bag){

	for(int i = 0; i < bag->numberOfItems; i++){

		if(bag->gitem[i] == item){

			bag->gitem[i] = NULL;
			bag->totalWeight -= item->weight;

			for(int j = i; j < bag->numberOfItems; j++){
				bag->gitem[j] = bag->gitem[j+1];
			}

			return 1;
		}
	}
	return 0;
}

// adds item to cart
int addItemToCart(GroceryItem *item, Cart *cart){
	
	cart->gitem[cart->numberOfItemsInCart] = item;
    	cart->numberOfItemsInCart++;
    	return 1;
}

// pack items in cart into bags
void packItemsIntoBags(Cart *cart){

    cart->packedGroceryBags = (Bag **) malloc (sizeof(Bag *) * cart->numberOfPackedBags);
      
	cart->packedGroceryBags[cart->numberOfPackedBags]= NULL;
	cart->packedGroceryBags[cart->numberOfPackedBags] = (Bag *) malloc(sizeof(Bag));
	initialiseBag(cart->packedGroceryBags[cart->numberOfPackedBags]);
	
    for(int i = 0; i < cart->numberOfItemsInCart; i++){

        int value = addItemToBag(cart->gitem[i], cart->packedGroceryBags[cart->numberOfPackedBags]);

    	if(value == 0){

			cart->numberOfPackedBags++;
			i--;

			cart->packedGroceryBags = (Bag **) realloc (cart->packedGroceryBags, sizeof(Bag *) * cart->numberOfPackedBags+1);
			cart->packedGroceryBags[cart->numberOfPackedBags]= NULL;
			cart->packedGroceryBags[cart->numberOfPackedBags] = (Bag *) malloc(sizeof(Bag));
			initialiseBag(cart->packedGroceryBags[cart->numberOfPackedBags]);
      	}
    }
}  

// prints grocery Item in proper format
void printGroceryItem(GroceryItem *item){
    printf("%s weighting %.3fkg with price $%.3f\n", item->name, item->weight, item->cost );
}

// returns the heaviest element in the bag
float findHeaviest(Bag *bag){

    float heavy = bag->gitem[0]->weight;

    for (int i = 1 ; i < bag->numberOfItems; i++){

        if(bag->gitem[i]->weight > heavy){
            heavy = bag->gitem[i]->weight;
        }
    }

    return heavy;
}

// prints the bag with proper format
void printGroceryBag(Bag *bag, int bagNumber){

    float heavy = findHeaviest(bag);

    printf("\nBag number %d (total weight = %.3f)\n",bagNumber,bag->totalWeight);

    for(int i = 0; bag->gitem[i]->cost > (float)0 && i < MAX_BAG_CAPACITY; i++){

		printf("\t");

        if(bag->gitem[i]->weight == heavy){
            printf("*");
        }
        printGroceryItem(bag->gitem[i]);
    }
}

// prints the cart with proper formating
void printCart(Cart *cart){

    // if there are any packedbag
    if(cart->numberOfPackedBags > 0){

        for(int i = 0; i < cart->numberOfItemsInCart; i++){

            if(cart->gitem[i]->weight > (float)MAX_BAG_WEIGHT){

                printGroceryItem(cart->gitem[i]);
            }
        }

        for(int j = 1; j <= cart->numberOfPackedBags; j++){
            printGroceryBag(cart->packedGroceryBags[j-1], (j));
        }

    }
    // if there are no packed bags then print all items as it is
    else{

        for(int a = 0; a < cart->numberOfItemsInCart; a++){

            printGroceryItem(cart->gitem[a]);
        }
    }
}



// removes teh perishable items from the cart and the bag
void removePerishItems(Cart *cart){

	for(int i = 0; i < cart->numberOfItemsInCart; i++){

		if(cart->gitem[i]->weight > (float)MAX_BAG_WEIGHT){

			if(cart->gitem[i]->isFrozen == 1){
				cart->gitem[i] = NULL;

				for(int j = i; j < cart->numberOfItemsInCart; j++){

					cart->gitem[i] = cart->gitem[i+1];
				}
			}
		}
	}

	for(int a = 0; a <= cart->numberOfPackedBags; a++){

		for( int b = 0; b < cart->packedGroceryBags[a]->numberOfItems; b++){

			if(cart->packedGroceryBags[a]->gitem[b]->isFrozen == 1){
				removeItemFromBag(cart->packedGroceryBags[a]->gitem[b], cart->packedGroceryBags[a]);
                b--;
                // if(cart->packedGroceryBags[a]->totalWeight == (float)0){
                //     printGroceryBag(cart->packedGroceryBags[a], a+1);
                //     printf("The Bag got empty\n");
                // }
			}
		}
	}

}


//MAIN FUNCTION 

void main(){

	Cart *cart;
	cart = (Cart *) malloc(sizeof(Cart));

	GroceryItem testItems[12];

    //Populating the array
	char *sampleItemNames[] = {"Smart-Ones Frozen Entrees", "SnackPack Pudding", "Breyers Chocolate Icecream", "Nabob Coffee", "Gold Seal Salmon", "Ocean Spray Cranberry Cocktail", "Heinz Beans Original", "Lean Ground Beef", "5-Alive Frozen Juice", "Coca-Cola 12-pack", "Toilet Paper - 48 pack", "Crate of milk"};
	float sampleItemPrices[] = {1.99, 0.99, 2.99, 3.99, 1.99, 2.99, 0.79, 4.94, 0.75, 3.49, 40.96, 12.99};
	float sampleItemWeights[] = {0.311, 0.396, 2.27, 0.326, 0.213, 2.26, 0.477, 0.75, 0.426, 5.112, 10.89f, 6.18f};
	float sampleItemPerish[] = {1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1};
	
    //using the random below
	srand(time(NULL));

    for(int i = 0; i < 12; i++)
    {
        testItems[i].name = sampleItemNames[i];
        testItems[i].cost = sampleItemPrices[i];
        testItems[i].weight = sampleItemWeights[i];
        testItems[i].isFrozen = sampleItemPerish[i];
    }

    initialiseCart(cart);

	int random;
	for (int i = 0; i < 50 ;i++){

		// gives the random between 0 and 11
		random  = (rand() % (12));
		addItemToCart(&testItems[random],cart);
	}

    // printing cart before pacakging 
	printf("\nPRINTING CART BEFORE PACKAGING\n\n");
	printCart(cart);

    cart->gitem = (GroceryItem **) realloc(cart->gitem, sizeof(GroceryItem*)*cart->numberOfItemsInCart);


   //packing the cart items into bags
    packItemsIntoBags(cart);

    // printing cart after packaging
	printf("\nPRINTING CART AFTER PACKAGING\n\n");
	printCart(cart);

	printf("\nPRINTING CART AFTER REMOVING PERISHABLE ITEMS\n\n");


    // Removes the perishable items from the cart and the bag
	removePerishItems(cart);

	printCart(cart);
    	

}