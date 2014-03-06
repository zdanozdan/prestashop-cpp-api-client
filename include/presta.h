#ifndef PRESTA_H_ 
#define PRESTA_H_
#include <string>

class PrestaShop
{
 public:
  //PrestaShop();
  PrestaShop(string url, string key);
  
  /**
   * Take the status code and throw an exception if the server didn't return 200 or 201 code
   * @param int status_code Status code of an HTTP return
   */
  int checkStatusCode(int status_code);

  /**
   * Handles a CURL request to PrestaShop Webservice
   * @param string url Resource name
   * @return response class/struct/map
   */
  PrestaResponse executeRequest(string url);
  /**
   * Load XML from string. Can throw exception
   * @param string $response String from a CURL response
   * @return SimpleXMLElement status_code, response
   */
  XML function parseXML(PrestaResponse response);

  /**
   * Add (POST) a resource
   * <p>Unique parameter must take : <br><br>
   * 'resource' => Resource name - path to api resource <br>
   * 'xml' => Full XML string to add resource<br><br>
   * Examples are given in the tutorial</p>
   * @param string resource
   * @param XML xml
   * @return XML
   */
  XML function add(string resource, XML xml);
  /**
   * Retrieve (GET) a resource
   * <p>Unique parameter must take : <br><br>
   * 'url' => Full URL for a GET request of Webservice (ex: http://mystore.com/api/customers/1/)<br>
   * OR<br>
   * 'resource' => Resource name,<br>
   * 'id' => ID of a resource you want to get<br><br>
   * </p>
   * @param string resource to get
   * @param int resource id
   * @return XML
   */
  XML function get(string resource, int id);

  /**
   * Head method (HEAD) a resource
   *
   * @param array $options Array representing resource for head request.
   * @return XML
   */
  XML function head();

  /**
   * Edit (PUT) a resource
   * <p>Unique parameter must take : <br><br>
   * 'resource' => Resource name ,<br>
   * 'id' => ID of a resource you want to edit,<br>
   * 'Xml' => Modified XML string of a resource<br><br>
   * @param string resource representing resource to edit.
   * @param int id resource ID.
   * @param XML xml to put
   */
  XML function edit(string resource, int id, XML xml);

  /**
   * Delete (DELETE) a resource.
   * Unique parameter must take : <br><br>
   * 'resource' => Resource name<br>
   * 'id' => ID or array which contains IDs of a resource(s) you want to delete<br><br>
   * @param string resource representing resource to delete.
   * @param int id resource id.
   */
  int function delete(string resource, int id)

  ~PrestaShop();
};
 
#endif
