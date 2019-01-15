open System

[<EntryPoint>]
let main argv =
    printfn "Welcome to fact calc!"
    printf "Input num: "
    let target = Console.ReadLine() |> int

    let rec fact target =
        match target with
        |0 -> 1
        |_ -> target * fact (target - 1)

    printfn "Input num: %d, Fact num: %d" target (fact target)

    0 // return an integer exit code
