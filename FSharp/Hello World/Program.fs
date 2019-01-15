// It looks like using System in C#
open System

// Declare main func
[<EntryPoint>]
let main argv =
    printfn "Hello World!"
    Console.ReadLine |> ignore
    0 // return an integer exit code
