import { Injectable, OnModuleDestroy, OnModuleInit } from '@nestjs/common';
import { PrismaClient } from '@prisma/client';

@Injectable()
export class PrismaService
    extends PrismaClient
    implements OnModuleInit, OnModuleDestroy
{
    constructor() {
        super({
            log: ['query']
        })
    }

    async onModuleInit() {
        this.$connect()
    }

    async onModuleDestroy() {
        this.$disconnect
    }
}
