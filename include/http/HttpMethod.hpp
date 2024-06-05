#ifndef HTTP_METHOD_HPP
#define HTTP_METHOD_HPP

#include <string>

enum HTTP_METHOD_CODE {
  GET,POST,PUT,DELETE,HEAD,OPTIONS,CONNECT,PATCH,TRACE
};

class HttpMethod
{
public:
  /// @brief GET: Retrieves a resource from the server. It's the most common method and is used to fetch data from the server. Example: GET /users
  inline static std::string GET = "GET";
  /// @brief POST: Creates a new resource on the server. It's often used to send data to the server to create a new resource. Example: POST /users
  inline static std::string POST = "POST";
  /// @brief  PUT: Updates an existing resource on the server. It's used to update a resource that already exists on the server. Example: PUT /users/1
  inline static std::string PUT = "PUT";
  /// @brief DELETE: Deletes a resource from the server. It's used to remove a resource from the server. Example: DELETE /users/1
  inline static std::string DELETE = "DELETE";
  /// @brief HEAD: Retrieves metadata about a resource from the server. It's similar to GET, but it only returns the headers, not the body. Example: HEAD /users
  inline static std::string HEAD = "HEAD";
  /// @brief OPTIONS: Returns the HTTP methods supported by the server for a specific resource. It's used to determine the capabilities of the server. Example: OPTIONS /users
  inline static std::string OPTIONS = "OPTIONS";
  /// @brief CONNECT: Establishes a tunnel to the server. It's used to create a proxy or a tunnel to the server. Example: CONNECT /tunnel
  inline static std::string CONNECT = "CONNECT";
  /// @brief PATCH: Partially updates an existing resource on the server. It's used to update a resource with a set of changes. Example: PATCH /users/1
  inline static std::string PATCH = "PATCH";
  /// @brief TRACE: Performs a message loop-back test along the path to the target resource. It's used to test the HTTP request and response cycle. Example: TRACE /users
  inline static std::string TRACE = "TRACE";

  static HTTP_METHOD_CODE getMethodCode(const std::string& method);
  
};

#endif