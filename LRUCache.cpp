#include <list>
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <random>
#include <ctime>
#include <iostream>
#include <iterator>

using namespace std;

struct ProductDetails
{
    int productId;
    std::string productName;
    std::string dscription;
    std::vector<uint8_t> image;
    std::vector<std::string> comments;
};

template <class KEY_T, class VAL_T> class LRUCache 
{
private:
    list< pair<KEY_T, VAL_T> > item_list;
    unordered_map<KEY_T, decltype(item_list.begin()) > item_map;
    size_t cache_size;
private:
    void clean(void) 
    {
        while (item_map.size() > cache_size) 
        {
            auto last_it = item_list.end(); last_it--;
            item_map.erase(last_it->first);
            item_list.pop_back();
        }
    };
public:
    LRUCache(int cache_size_) :cache_size(cache_size_){};

    void put(const KEY_T& key, const VAL_T& val) 
    {
        auto it = item_map.find(key);
        if (it != item_map.end()) 
        {
            item_list.erase(it->second);
            item_map.erase(it);
        }
        item_list.push_front(make_pair(key, val));
        item_map.insert(make_pair(key, item_list.begin()));
        clean();
    };
    bool exist(const KEY_T& key) 
    {
        return (item_map.count(key) > 0);
    };
    VAL_T get(const KEY_T& key) 
    {
        assert(exist(key));
        auto it = item_map.find(key);
        item_list.splice(item_list.begin(), item_list, it->second);
        return it->second->second;
    }
};

class DBDriver 
{
    public:
        ProductDetails fetchProductDetails(int i)
        {
            return products[i];
        }
        std::vector<int> getProductIds()
        {
            std::vector<int> ids;
            for (int i = 0; i <= products.size(); i++)
            {
                ids.push_back(products[i].productId);
            }
            return ids;
        }
    private:
        std::vector<ProductDetails> products;
};

std::vector<ProductDetails> GenerateDB(int size)
{
		std::vector<ProductDetails> products;

		for (int i = 0; i <= size; i++)
		{
            std::srand(std::time(nullptr));
            ProductDetails details;
            details.productId = i;
            details.productName = "Product" + std::to_string(i);
            details.dscription = "This is a description for Product " + std::to_string(i);        
            details.image.resize(512);
            std::generate(details.image.begin(), details.image.end(), [](){ return std::rand() % 256; });                 
            for (int j = 0; j <= 512; j++)
            {
                details.comments.push_back("Comment" + std::to_string(j) + "for product");
            }           
            products.push_back(std::move(details));
		}
		return products;
}

int main() 
{  
   std::vector<ProductDetails> prod = GenerateDB(3);

   LRUCache< int, std::string> cache(2);

        //cache has size 2, add two items
   cache.put(prod[1].productId, prod[1].productName);
   cache.put(prod[2].productId, prod[2].productName);

        //print the value of the first element
   std::cout << "productId: " << cache.get(prod[1].productId) << std::endl;

        //add a third element
   cache.put(prod[3].productId, prod[3].productName);

        //trying to display the second element, but it is no longer in the cache
   std::cout << "productId: " << cache.get(prod[2].productId) << std::endl;

    return 0;
 }

