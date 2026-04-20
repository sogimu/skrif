{
    function createCounter(start) {
        var count = start;
        function increment() {
            count = count + 1;
            return count;
        }
        return increment;
    }

    var counter = createCounter(0);
    var i = 0;
    var result = 0;
    while (i < 3000) {
        result = counter();
        i = i + 1;
    }
    print(result);
}
