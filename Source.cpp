#include <string>
#include <cwchar>

void
ArgvQuote(
    const std::wstring& Argument,
    std::wstring& CommandLine,
    bool Force
)
/*++
Routine Description:
    This routine appends the given argument to a command line such
    that CommandLineToArgvW will return the argument string unchanged.
    Arguments in a command line should be separated by spaces; this
    function does not add these spaces.
Arguments:
    Argument - Supplies the argument to encode.
    CommandLine - Supplies the command line to which we append the encoded argument string.
    Force - Supplies an indication of whether we should quote
            the argument even if it does not contain any characters that would
            ordinarily require quoting.
Return Value:
    None.
Environment:
    Arbitrary.
--*/
{
    //
    // Unless we're told otherwise, don't quote unless we actually
    // need to do so --- hopefully avoid problems if programs won't
    // parse quotes properly
    //
    if (Force == false &&
        Argument.empty() == false &&
        Argument.find_first_of(L" \t\n\v\"") == Argument.npos)
    {
        CommandLine.append(Argument);
    }
    else {
        CommandLine.push_back(L'"');
        for (auto It = Argument.begin(); ; ++It) {
            unsigned NumberBackslashes = 0;
            while (It != Argument.end() && *It == L'\\') {
                ++It;
                ++NumberBackslashes;
            }
            if (It == Argument.end()) {
                //
                // Escape all backslashes, but let the terminating
                // double quotation mark we add below be interpreted
                // as a metacharacter.
                //
                CommandLine.append(NumberBackslashes * 2, L'\\');
                break;
            }
            else if (*It == L'"') {
                //
                // Escape all backslashes and the following
                // double quotation mark.
                //
                CommandLine.append(NumberBackslashes * 2 + 1, L'\\');
                CommandLine.push_back(*It);
            }
            else {
                //
                // Backslashes aren't special here.
                //
                CommandLine.append(NumberBackslashes, L'\\');
                CommandLine.push_back(*It);
            }
        }
        CommandLine.push_back(L'"');
    }
}

int main()
{
    wchar_t OneArg[1000];
    std::wstring CommandLine;

    for (int i = 0; ; i++) {
        std::wstring CurrArg;

        if (i) {
            CommandLine.push_back(L' ');
        }

        wprintf_s(L"Please input arg[%d]: ", i);
        _getws_s(OneArg);
        CurrArg.assign(OneArg);
        if (CurrArg.size() == 0) break;

        ArgvQuote(CurrArg, CommandLine, false);
    }

    wprintf_s(L"\n%ls\n", CommandLine.c_str());
    return 0;
}