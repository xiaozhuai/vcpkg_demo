import url from 'node:url';
import path from 'node:path';
import { transformAsync } from '@babel/core';

const __dirname = path.dirname(url.fileURLToPath(import.meta.url));

export default {
    input: path.join(__dirname, 'src', 'index.js'),
    output: {
        file: path.join(__dirname, 'dist', 'index.js'),
        format: 'iife',
        generatedCode: 'es5',
        strict: true,
    },
    plugins: [
        {
            name: 'babel-es5',
            async transform(code, id) {
                if (!id.endsWith('.js') || id.includes('/node_modules/')) {
                    return null;
                }
                const result = await transformAsync(code, {
                    filename: id,
                    sourceType: 'module',
                    babelrc: false,
                    configFile: false,
                    sourceMaps: true,
                    // Babel 8 defaults to modern browsers. Explicit legacy
                    // targets also keep its generated helpers ES5-compatible.
                    targets: '>= 0%',
                    browserslistConfigFile: false,
                    presets: [
                        [
                            '@babel/preset-env',
                            {
                                forceAllTransforms: true,
                                // Let Rollup link ESM and enclose Babel's helpers.
                                modules: false,
                                exclude: ['transform-typeof-symbol'],
                            },
                        ],
                    ],
                });
                return { code: result.code, map: result.map };
            },
        },
    ],
};
