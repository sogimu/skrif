{
    function fib(n) {
        var a = 0;
        var b = 1;
        var tmp = 0;
        var i = 0;
        while (i < n) {
            tmp = b;
            b = a + b;
            a = tmp;
            i = i + 1;
        }
        return a;
    }

    print(fib(5000));
}
