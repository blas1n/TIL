open System

type Person = { First: string; Last: string }

type Employee =
    | Worker of Person
    | Manager of Employee list

[<EntryPoint>]
let main argv =
// This is one line comment
(* This is multi line comment
*)

// make value (immutable)
    let five = 5
    let pi = 3.14
    let hello = "hello"

// make list, :: is make new element, @ is append list
    let twoToFive = [2;3;4;5] 
    let oneToFive = 1 :: twoToFive
    let zeroToFive = [0;1] @ twoToFive

// make func before = is param, after = is execution Statement
// ignore is to throw away value
    let square x = x * x
    square 3 |> ignore
    let add x y = x + y
    add 2 3 |> ignore

// F# grammar looks like python (because use indent), and return last syntax
    let evens list =
        let isEven x = x%2 = 0
        List.filter isEven list

    let evensInOneToFive = evens oneToFive
    
// output 2, 4
    printfn "%A" evensInOneToFive

// parenthesis control sequence
    let sumOfSquaresTo100 = 
        List.sum(List.map square [1..100])

// pipe_forward operator looks like method chaining in object-oriented
    let sumOfSquaresTo100piped = 
        [1..100] |> List.map square |> List.sum

    let sumOfSquaresTo100withLamda =
        [1..100] |> List.map(fun x->x*x) |> List.sum

// This is pattern match. last match looks like default in switch-case
    let simplePatternMatch = 
        let x = "a"

        match x with
        |"a" -> printfn "x is a"
        |"b" -> printfn "x is b"
        |_ -> printfn "x is something else"

    let validValue = Some(99)
    let invalidValue = None

    let optionPatternMatch input =
        match input with
        |Some i -> printfn "input is an int = %d" i
        |None -> printfn "input is null"

    optionPatternMatch validValue
    optionPatternMatch invalidValue

// this is tuple (similar python tuple)
    let twoTuple = 1,2
    let threeTuple = "a",2,true

    let person1 = { First="john"; Last="Doe" }

    let jdoe = { First="John"; Last="Doe" }
    let worker = Worker jdoe

    printfn "Printing an int %i, a float %f, a bool %b" 1 2.0 true
    printfn "A string %s, and something generic %A" "hello" [1..4]

    printfn "twoTuple=%A,\nPerson=%A,\nTemp=%A,\nEmployee=%A" twoTuple person1 worker worker

    0 // return an integer exit code
