extends Resource

## details format
## "currencyTexture" : Texture2D
## "currencyReference" : Equipment
## "shopContents" : Dictionary ->
## "<column name 1>" : Dictionary ->
## "<purchasable 1>" : <cost>
## "<purchasable 2>" : <cost>
## ...
## "<column name 2>" : Dictionary ->
## ...
## ...
class_name ShopDetails
@export var shopName : String = ""
@export var shopCurrencyTexture : Texture2D
@export var shopCurrency : Equipment
@export var shopContents : Array[ShopColumn] = [ShopColumn.new()]

#func addPurchasable(type : String, price : int, myName : String, ref) :
	#if (det["shopContents"].get(type) == null) :
		#det["shopContents"][type] = {}
	#det["shopContents"][type][myName] = createPurchasable(price, ref)
#func setShopDetails(shopCon : Dictionary) :
	#det["shopContents"] = shopCon
#static func createShopContentsFromColumArray(columnNames : Array[String], columns : Array[Dictionary]) :
	#var retVal = {}
	#for index in range(0, columnNames.size()) :
		#retVal[columnNames[index]] = columns[index]
	#return retVal
#static func createShopColumnFromPurchasableArray(purchasableNames : Array[String], purchasables : Array[Dictionary]) :
	#var retVal = {}
	#for index in range(0,purchasableNames.size()) :
		#retVal[purchasableNames[index]] = purchasables[index]
	#return retVal
#static func createPurchasable(price : int, ref) :
	#return {
		#"price" : price,
		#"ref" : ref
	#}
#func getDetails() :
	#return det
#func getCurrencyTexture() :
	#return det["currencyTexture"]
#func getCurrencyReference() :
	#return det["currencyReference"]
#func getColumnNameArray() :
	#return det["shopContents"].keys()
#func getShopColumn(columnName : String) :
	#return det["shopContents"][columnName]
#func getShopColumnArray() :
	#return det["shopContents"]
