#include <gtest/gtest.h>

#include <memory>

#include "../src/Common/Exceptions.hpp"
#include "../src/Interpreter.hpp"
#include "../src/Lexer.hpp"
#include "../src/Parser.hpp"
#include "../src/Runtime/Context.hpp"

using namespace o2l;

class UrlLibraryTest : public ::testing::Test {
   protected:
    Context context;

    // Helper method to evaluate O²L code
    Value evaluateCode(const std::string& code) {
        Lexer lexer(code);
        auto tokens = lexer.tokenizeAll();

        Parser parser(tokens);
        auto ast = parser.parse();

        Interpreter interpreter;
        return interpreter.execute(ast);
    }

    // Helper to check text results
    void expectText(const Value& result, const std::string& expected) {
        ASSERT_TRUE(std::holds_alternative<Text>(result));
        EXPECT_EQ(std::get<Text>(result), expected);
    }

    // Helper to check boolean results
    void expectBool(const Value& result, bool expected) {
        ASSERT_TRUE(std::holds_alternative<Bool>(result));
        EXPECT_EQ(std::get<Bool>(result), expected);
    }
};

// URL validation tests
TEST_F(UrlLibraryTest, BasicUrlValidation) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Bool {
                return url.isValid("https://example.com/path")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectBool(result, true);
}

TEST_F(UrlLibraryTest, InvalidUrlValidation) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Bool {
                return url.isValid("not-a-url")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectBool(result, false);
}

// URL parsing tests
TEST_F(UrlLibraryTest, GetSchemeFromUrl) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getScheme("https://example.com/path")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https");
}

TEST_F(UrlLibraryTest, GetHostFromUrl) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getHost("https://example.com:8080/path")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "example.com");
}

TEST_F(UrlLibraryTest, GetPortFromUrl) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getPort("https://example.com:8080/path")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "8080");
}

TEST_F(UrlLibraryTest, GetPortFromUrlWithoutPort) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getPort("https://example.com/path")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "");
}

TEST_F(UrlLibraryTest, GetPathFromUrl) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getPath("https://example.com/api/users")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "/api/users");
}

TEST_F(UrlLibraryTest, GetQueryFromUrl) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getQuery("https://example.com/search?q=test&limit=10")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "q=test&limit=10");
}

TEST_F(UrlLibraryTest, GetFragmentFromUrl) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getFragment("https://example.com/page#section1")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "section1");
}

// URL construction tests
TEST_F(UrlLibraryTest, CreateBasicUrl) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.create("https", "example.com")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com/");
}

TEST_F(UrlLibraryTest, CreateUrlWithPort) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.create("https", "example.com", "8080")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com:8080/");
}

TEST_F(UrlLibraryTest, CreateCompleteUrl) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.create("https", "example.com", "8080", "/api/users", "limit=10", "results")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com:8080/api/users?limit=10#results");
}

// URL modification tests
TEST_F(UrlLibraryTest, SetScheme) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.setScheme("http://example.com/path", "https")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com/path");
}

TEST_F(UrlLibraryTest, SetHost) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.setHost("https://old.com/path", "new.com")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://new.com/path");
}

TEST_F(UrlLibraryTest, SetPortWithText) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.setPort("https://example.com/path", "9000")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com:9000/path");
}

TEST_F(UrlLibraryTest, SetPath) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.setPath("https://example.com/old", "/new/path")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com/new/path");
}

TEST_F(UrlLibraryTest, SetQuery) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.setQuery("https://example.com/path", "new=query")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com/path?new=query");
}

TEST_F(UrlLibraryTest, SetFragment) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.setFragment("https://example.com/path", "newfragment")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com/path#newfragment");
}

// Query parameter tests
TEST_F(UrlLibraryTest, GetQueryParameter) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getParam("https://example.com/search?q=test&limit=10", "q")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "test");
}

TEST_F(UrlLibraryTest, GetNonExistentParameter) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getParam("https://example.com/search?q=test", "missing")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "");
}

TEST_F(UrlLibraryTest, SetQueryParameter) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.setParam("https://example.com/search", "q", "newvalue")
            }
        }
    )";

    Value result = evaluateCode(code);
    // The exact order of parameters may vary, so we check that it contains both the base URL and
    // parameter
    ASSERT_TRUE(std::holds_alternative<Text>(result));
    std::string resultStr = std::get<Text>(result);
    EXPECT_TRUE(resultStr.find("https://example.com/search") != std::string::npos);
    EXPECT_TRUE(resultStr.find("q=newvalue") != std::string::npos);
}

TEST_F(UrlLibraryTest, RemoveQueryParameter) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.removeParam("https://example.com/search?q=test&limit=10", "q")
            }
        }
    )";

    Value result = evaluateCode(code);
    ASSERT_TRUE(std::holds_alternative<Text>(result));
    std::string resultStr = std::get<Text>(result);
    EXPECT_TRUE(resultStr.find("limit=10") != std::string::npos);
    EXPECT_TRUE(resultStr.find("q=test") == std::string::npos);
}

TEST_F(UrlLibraryTest, GetAllParameters) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getParams("https://example.com/search?q=test&limit=10")
            }
        }
    )";

    Value result = evaluateCode(code);
    ASSERT_TRUE(std::holds_alternative<Text>(result));
    std::string resultStr = std::get<Text>(result);
    // Should return something like {"q": "test", "limit": "10"}
    EXPECT_TRUE(resultStr.find("q") != std::string::npos);
    EXPECT_TRUE(resultStr.find("test") != std::string::npos);
    EXPECT_TRUE(resultStr.find("limit") != std::string::npos);
    EXPECT_TRUE(resultStr.find("10") != std::string::npos);
}

// URL manipulation tests
TEST_F(UrlLibraryTest, NormalizeUrl) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.normalize("HTTPS://EXAMPLE.COM/Path/../Another")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com/Another");
}

TEST_F(UrlLibraryTest, ResolveAbsoluteUrl) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.resolve("https://example.com/base/", "https://other.com/absolute")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://other.com/absolute");
}

TEST_F(UrlLibraryTest, ResolveRelativeUrl) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.resolve("https://example.com/base/path/", "relative")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com/base/path/relative");
}

TEST_F(UrlLibraryTest, ResolveAbsolutePath) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.resolve("https://example.com/base/path/", "/newpath")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com/newpath");
}

TEST_F(UrlLibraryTest, JoinUrls) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.join("https://example.com", "api", "users", "123")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com/api/users/123");
}

// URL encoding/decoding tests
TEST_F(UrlLibraryTest, EncodeText) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.encode("hello world!")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "hello%20world%21");
}

TEST_F(UrlLibraryTest, DecodeText) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.decode("hello%20world%21")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "hello world!");
}

// Utility tests
TEST_F(UrlLibraryTest, GetDomain) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getDomain("https://www.example.com/path")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "example.com");
}

TEST_F(UrlLibraryTest, GetDomainSimple) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getDomain("https://example.com/path")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "example.com");
}

TEST_F(UrlLibraryTest, GetOrigin) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getOrigin("https://example.com:8080/path?query=1#fragment")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com:8080");
}

TEST_F(UrlLibraryTest, GetOriginWithDefaultPort) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getOrigin("https://example.com:443/path")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com");
}

TEST_F(UrlLibraryTest, IsAbsoluteUrl) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Bool {
                return url.isAbsolute("https://example.com/path")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectBool(result, true);
}

TEST_F(UrlLibraryTest, IsRelativeUrl) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Bool {
                return url.isRelative("/path/to/resource")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectBool(result, true);
}

TEST_F(UrlLibraryTest, IsRelativeUrlWithDotPath) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Bool {
                return url.isRelative("./relative/path")
            }
        }
    )";

    Value result = evaluateCode(code);
    expectBool(result, true);
}

// Error handling tests
TEST_F(UrlLibraryTest, InvalidSchemeError) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.create("123invalid", "example.com")
            }
        }
    )";

    EXPECT_THROW(evaluateCode(code), EvaluationError);
}

TEST_F(UrlLibraryTest, InvalidHostError) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.create("https", "")
            }
        }
    )";

    EXPECT_THROW(evaluateCode(code), EvaluationError);
}

TEST_F(UrlLibraryTest, GetSchemeFromInvalidUrl) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                return url.getScheme("not-a-valid-url")
            }
        }
    )";

    EXPECT_THROW(evaluateCode(code), EvaluationError);
}

// Complex real-world scenarios
TEST_F(UrlLibraryTest, ComplexUrlManipulation) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                base_url: Text = "https://api.example.com/v1/users"
                with_params: Text = url.setParam(base_url, "limit", "50")
                with_more_params: Text = url.setParam(with_params, "offset", "100")
                final_url: Text = url.setParam(with_more_params, "fields", "name,email")
                return final_url
            }
        }
    )";

    Value result = evaluateCode(code);
    ASSERT_TRUE(std::holds_alternative<Text>(result));
    std::string resultStr = std::get<Text>(result);
    EXPECT_TRUE(resultStr.find("https://api.example.com/v1/users") != std::string::npos);
    EXPECT_TRUE(resultStr.find("limit=50") != std::string::npos);
    EXPECT_TRUE(resultStr.find("offset=100") != std::string::npos);
    EXPECT_TRUE(resultStr.find("fields=name%2Cemail") != std::string::npos);
}

TEST_F(UrlLibraryTest, UrlParsingAndReconstruction) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                original: Text = "https://example.com:8080/api/users?limit=10&offset=0#results"
                
                scheme: Text = url.getScheme(original)
                host: Text = url.getHost(original)
                port: Text = url.getPort(original)
                path: Text = url.getPath(original)
                query: Text = url.getQuery(original)
                fragment: Text = url.getFragment(original)
                
                return url.create(scheme, host, port, path, query, fragment)
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com:8080/api/users?limit=10&offset=0#results");
}

TEST_F(UrlLibraryTest, UrlNormalizationWithComplexPath) {
    std::string code = R"(
        import url
        
        Object Main {
            method main(): Text {
                messy_url: Text = "HTTPS://Example.COM:443/api/../users/./profile/../settings"
                return url.normalize(messy_url)
            }
        }
    )";

    Value result = evaluateCode(code);
    expectText(result, "https://example.com/users/settings");
}