import { IsNumber, IsString, Min } from "class-validator";

export class CreateProductDto {
    @IsString()
    name: string;

    @IsNumber()
    @Min(0)
    price: number
}
