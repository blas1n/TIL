import { Query, Resolver } from '@nestjs/graphql';
import { ProductService } from './product.service';

@Resolver('Product')
export class ProductResolver {
    constructor(private productService : ProductService) {}

    @Query()
    async findAll() {
        return this.productService.findAll()
    }
}
