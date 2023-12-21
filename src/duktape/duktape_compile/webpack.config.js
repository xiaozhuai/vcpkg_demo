const pth = require('path');
module.exports = {
    entry: pth.join(__dirname, 'src', 'index.js'),
    output: {
        path: pth.join(__dirname, 'lib'),
        filename: 'index.js'
    },
    mode: 'production',
    target: ['node', 'es5'],
    module: {
        rules: [
            {
                test: /\.js$/,
                exclude: /node_modules/,
                use: {
                    loader: 'babel-loader',
                    options: {
                        presets: [
                            ['@babel/preset-env', {
                                bugfixes: true,
                                exclude: [
                                    'transform-typeof-symbol'
                                ],
                                // useBuiltIns: 'usage',
                                // corejs: 3,
                                modules: 'commonjs',
                            }],
                        ],
                        plugins: [
                            '@babel/plugin-proposal-object-rest-spread',
                        ]
                    }
                }
            }
        ]
    }
};
