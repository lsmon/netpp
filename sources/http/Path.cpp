//
// Created by alcacruz on 11/21/24.
//

#include "http/Path.hpp"

void Path::compilePattern(const std::string &pathPattern) {
    std::regex keyRegex("\\{([^}]+)\\}"); // Matches placeholders like {key1}
    std::string regexString;
    size_t lastPos = 0;

    // Iterate over placeholders in the pattern
    for (std::sregex_iterator it(pathPattern.begin(), pathPattern.end(), keyRegex), end; it != end; ++it) {
        std::smatch match = *it;

        // Append the literal part before the placeholder
        regexString += std::regex_replace(
                pathPattern.substr(lastPos, match.position() - lastPos),
                std::regex("/"), "\\/"
        );

        // Add a capture group for the placeholder
        regexString += "([^/]+)";

        // Store the key name
        paramKeys.push_back(match[1]);

        // Update the last processed position
        lastPos = match.position() + match.length();
    }

    // Append any remaining literal part after the last placeholder
    regexString += std::regex_replace(pathPattern.substr(lastPos), std::regex("/"), "\\/");

    // Compile the final regex pattern with strict matching
    regexPattern = std::regex("^" + regexString + "$");
}

void Path::setPathPattern(const std::string &pathPattern) {
    compilePattern(pathPattern);
}

Path::Path(const std::string &pathPattern) : pattern(pathPattern) {
    compilePattern(pathPattern);
}

bool Path::match(const std::string &path) {
    std::smatch match;
    if (!std::regex_match(path, match, regexPattern)) {
        return false; // Does not match the pattern
    }

    // If matched, validate segments
    if (match.size() - 1 != paramKeys.size()) {
        return false; // Mismatch in number of placeholders
    }

    for (size_t i = 0; i < paramKeys.size(); ++i) {
        std::string key = paramKeys[i];
        pathParamMap[key] = match[i+1];
    }
    // Perform additional validation if needed
    return true;
}

const std::map<std::string, std::string> &Path::getPathParamMap() const {
    return pathParamMap;
}
